#include "src/db/operations/operationsdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr int MIN_FILE_SIZE_PARALLEL = 5000;



OperationsDatabase::OperationsDatabase(
    IDirFactory* dirFactory, IFileFactory* fileFactory, ILogosStorage* logosStorage, bool autoPilotMode
) :
    IOperationsDatabase(),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory),
    mLogosStorage(logosStorage),
    mAutoPilotMode(autoPilotMode),
    mAccountHash()
{
    qDebug() << "Create OperationsDatabase";
}

OperationsDatabase::~OperationsDatabase()
{
    qDebug() << "Destroy OperationsDatabase";
}

void OperationsDatabase::setAccount(const QString& account)
{
    mAccountHash = account;
}

struct FindOperationsIndeciesInfo
{
    explicit FindOperationsIndeciesInfo(const QByteArray& _content) :
        content(_content)
    {
        const int cpuCount = QThread::idealThreadCount();

        results.resize(cpuCount);
    }

    QByteArray        content;
    QList<QList<int>> results;
};

static void findOperationsIndeciesForParallel(
    QThread* parentThread, int threadId, QList<int>& /*temp*/, int start, int end, void* additionalArgs
)
{
    FindOperationsIndeciesInfo* findOperationsIndeciesInfo = reinterpret_cast<FindOperationsIndeciesInfo*>(additionalArgs);

    const char* contentArray = findOperationsIndeciesInfo->content.constData();
    QList<int>* resultsArray = findOperationsIndeciesInfo->results.data();

    const int part = findOperationsIndeciesInfo->content.size() / findOperationsIndeciesInfo->results.size();

    start = part * threadId;
    end   = qMin(part * (threadId + 1), findOperationsIndeciesInfo->content.size() - 3);

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        if (contentArray[i] == '}' && contentArray[i + 1] == ',' &&
            (contentArray[i + 2] == '\n' || (contentArray[i + 2] == '\r' && contentArray[i + 3] == '\n')))
        {
            resultsArray[threadId].append(i);

            i += 3;
        }
    }
}

struct MergeOperationsIndeciesInfo
{
    explicit MergeOperationsIndeciesInfo(const QList<QList<int>>& _results) :
        results(_results)
    {
        indecies.resizeForOverwrite(results.size() + 1);

        int index = 0;

        for (int i = 0; i < results.size(); ++i)
        {
            indecies[i] = index;

            index += results.at(i).size();
        }

        indecies[results.size()] = index;
    }

    QList<int>        indecies;
    QList<QList<int>> results;
};

static void mergeOperationsIndeciesForParallel(
    QThread* parentThread, int threadId, QList<int>& res, int /*start*/, int /*end*/, void* additionalArgs
)
{
    MergeOperationsIndeciesInfo* mergeOperationsIndeciesInfo = reinterpret_cast<MergeOperationsIndeciesInfo*>(additionalArgs);

    const int         index   = mergeOperationsIndeciesInfo->indecies.at(threadId);
    const QList<int>& results = mergeOperationsIndeciesInfo->results.at(threadId);

    int* resArray = res.data();

    for (int i = 0; i < results.size() && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[index + i] = results.at(i);
    }
}

struct ReadOperationsInfo
{
    explicit ReadOperationsInfo(ILogosStorage* _logosStorage, const QByteArray& _content, QList<int>* _indecies) :
        logosStorage(_logosStorage),
        content(_content),
        indecies(_indecies)
    {
    }

    ILogosStorage* logosStorage;
    QByteArray     content;
    QList<int>*    indecies;
};

static void readOperationsForParallel(
    QThread* parentThread, int /*threadId*/, QList<Operation>& res, int start, int end, void* additionalArgs
)
{
    ReadOperationsInfo* readOperationsInfo = reinterpret_cast<ReadOperationsInfo*>(additionalArgs);

    ILogosStorage*   logosStorage  = readOperationsInfo->logosStorage;
    const QByteArray content       = readOperationsInfo->content;
    int*             indeciesArray = readOperationsInfo->indecies->data();

    simdjson::ondemand::parser parser;

    Operation* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Operation& operation = resArray[res.size() - i - 1];

        const int startBlock = i > 0 ? indeciesArray[i - 1] + 3 : 0;
        const int endBlock   = indeciesArray[i];

        const QByteArray        operationContent = content.mid(startBlock, endBlock - startBlock + 1);
        simdjson::padded_string jsonData(operationContent.toStdString());

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            operation.fromJsonObject(doc.get_object());

            logosStorage->lock(); // TODO: Should do read lock outside
            operation.instrumentLogo = logosStorage->getLogo(operation.instrumentId);
            logosStorage->unlock();
        }
        catch (...)
        {
            qWarning() << "Failed to parse operation";
        }
    }
}

QList<Operation> OperationsDatabase::readOperations()
{
    qDebug() << "Reading operations from database";

    QList<Operation> res;

    const std::shared_ptr<IFile> operationsFile = mFileFactory->newInstance(operationsDirPath() + "/operations.json");

    if (operationsFile->open(QIODevice::ReadOnly))
    {
        QByteArray content = operationsFile->readAll();
        operationsFile->close();

        if (content.size() > MIN_FILE_SIZE_PARALLEL)
        {
            QList<int> indecies;

            FindOperationsIndeciesInfo findOperationsIndeciesInfo(content);
            processInParallel(indecies, findOperationsIndeciesForParallel, &findOperationsIndeciesInfo);

            MergeOperationsIndeciesInfo mergeOperationsIndeciesInfo(findOperationsIndeciesInfo.results);
            indecies.resizeForOverwrite(mergeOperationsIndeciesInfo.indecies.constLast() + 1);
            processInParallel(indecies, mergeOperationsIndeciesForParallel, &mergeOperationsIndeciesInfo);

            indecies[indecies.size() - 1] = content.size() - 1;

            res.resizeForOverwrite(indecies.size());

            ReadOperationsInfo readOperationsInfo(mLogosStorage, content, &indecies);
            processInParallel(res, readOperationsForParallel, &readOperationsInfo);
        }
        else
        {
            content = "[" + content + "]";
            simdjson::padded_string jsonData(content.toStdString());

            simdjson::ondemand::parser parser;

            try
            {
                simdjson::ondemand::document doc = parser.iterate(jsonData);

                simdjson::ondemand::array jsonOperations = doc.get_array();
                res.resizeForOverwrite(jsonOperations.count_elements());

                int i = res.size() - 1;

                mLogosStorage->lock();

                for (simdjson::ondemand::object jsonObject : jsonOperations)
                {
                    Operation& operation = res[i];

                    operation.fromJsonObject(jsonObject);
                    operation.instrumentLogo = mLogosStorage->getLogo(operation.instrumentId);

                    --i;
                }

                mLogosStorage->unlock();
            }
            catch (...)
            {
                qWarning() << "Failed to parse operations";
            }
        }
    }

    return res;
}

void OperationsDatabase::writeOperations(const QList<Operation>& operations)
{
    qDebug() << "Writing operations to database";

    const QString dirPath = operationsDirPath();

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(dirPath);
    Q_ASSERT_X(ok, __FUNCTION__ "()", "Failed to create dir");

    const std::shared_ptr<IFile> operationsFile = mFileFactory->newInstance(dirPath + "/operations.json");

    ok = operationsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__ "()", "Failed to open file");

    for (int i = 0; i < operations.size(); ++i)
    {
        const QJsonDocument jsonDoc(operations.at(operations.size() - i - 1).toJsonObject());

        if (i != 0)
        {
            operationsFile->write(",\n");
        }

        operationsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    }

    operationsFile->close();
}

void OperationsDatabase::appendOperations(const QList<Operation>& operations)
{
    qDebug() << "Appending operations to database";

    const std::shared_ptr<IFile> operationsFile = mFileFactory->newInstance(operationsDirPath() + "/operations.json");

    const bool ok = operationsFile->open(QIODevice::Append);
    Q_ASSERT_X(ok, __FUNCTION__ "()", "Failed to open file");

    for (int i = 0; i < operations.size(); ++i)
    {
        const QJsonDocument jsonDoc(operations.at(operations.size() - i - 1).toJsonObject());

        operationsFile->write(",\n");
        operationsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    }

    operationsFile->close();
}

void OperationsDatabase::setAutoPilotMode(bool autoPilotMode)
{
    mAutoPilotMode = autoPilotMode;
}

QString OperationsDatabase::operationsDirPath() const
{
    QString res;

    if (mAutoPilotMode)
    {
        res = QString("%1/data/autopilot/%2").arg(qApp->applicationDirPath(), mAccountHash);
    }
    else
    {
        res = QString("%1/data/simulator").arg(qApp->applicationDirPath());
    }

    return res;
}
