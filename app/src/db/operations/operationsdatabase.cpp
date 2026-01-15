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
    explicit FindOperationsIndeciesInfo(const QByteArray& _content)
    {
        contentArray = _content.constData();
        contentSize  = _content.size();

        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    const char*       contentArray;
    qint64            contentSize;
    QList<QList<int>> results;
    QList<int>*       resultsArray;
};

static void findOperationsIndeciesForParallel(
    QThread* parentThread, int threadId, int* /*temp*/, int /*size*/, int start, int end, void* additionalArgs
)
{
    const FindOperationsIndeciesInfo* findOperationsIndeciesInfo = reinterpret_cast<FindOperationsIndeciesInfo*>(additionalArgs);

    const char*  contentArray = findOperationsIndeciesInfo->contentArray;
    const qint64 contentSize  = findOperationsIndeciesInfo->contentSize;
    QList<int>*  resultsArray = findOperationsIndeciesInfo->resultsArray;
    const int    resultsSize  = findOperationsIndeciesInfo->results.size();

    const int part = contentSize / resultsSize;

    start = part * threadId;
    end   = qMin(part * (threadId + 1), contentSize - 3);

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
    explicit MergeOperationsIndeciesInfo(const QList<QList<int>>& _results)
    {
        indecies.resizeForOverwrite(_results.size() + 1);
        indeciesArray = indecies.data();
        resultsArray  = _results.constData();

        int index = 0;

        for (int i = 0; i < _results.size(); ++i)
        {
            indeciesArray[i] = index;

            index += resultsArray[i].size();
        }

        indeciesArray[_results.size()] = index;
    }

    QList<int>        indecies;
    int*              indeciesArray;
    const QList<int>* resultsArray;
};

static void mergeOperationsIndeciesForParallel(
    QThread* parentThread, int threadId, int* res, int /*size*/, int /*start*/, int /*end*/, void* additionalArgs
)
{
    const MergeOperationsIndeciesInfo* mergeOperationsIndeciesInfo =
        reinterpret_cast<MergeOperationsIndeciesInfo*>(additionalArgs);

    const int         index   = mergeOperationsIndeciesInfo->indeciesArray[threadId];
    const QList<int>& results = mergeOperationsIndeciesInfo->resultsArray[threadId];

    const int* resultsArray = results.constData();
    const int  resultsSize  = results.size();

    for (int i = 0; i < resultsSize && !parentThread->isInterruptionRequested(); ++i)
    {
        res[index + i] = resultsArray[i];
    }
}

struct ReadOperationsInfo
{
    explicit ReadOperationsInfo(ILogosStorage* _logosStorage, const QByteArray& _content, const QList<int>& _indecies) :
        logosStorage(_logosStorage),
        content(_content)
    {
        indeciesArray = _indecies.constData();
    }

    ILogosStorage* logosStorage;
    QByteArray     content;
    const int*     indeciesArray;
};

static void readOperationsForParallel(
    QThread* parentThread, int /*threadId*/, Operation* res, int size, int start, int end, void* additionalArgs
)
{
    const ReadOperationsInfo* readOperationsInfo = reinterpret_cast<ReadOperationsInfo*>(additionalArgs);

    ILogosStorage*    logosStorage  = readOperationsInfo->logosStorage;
    const QByteArray& content       = readOperationsInfo->content;
    const int*        indeciesArray = readOperationsInfo->indeciesArray;

    simdjson::ondemand::parser parser;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Operation& operation = res[size - i - 1];

        const int startBlock = i > 0 ? indeciesArray[i - 1] + 3 : 0;
        const int endBlock   = indeciesArray[i];

        const QByteArray              operationContent = content.mid(startBlock, endBlock - startBlock + 1);
        const simdjson::padded_string jsonData(operationContent.toStdString());

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            operation.fromJsonObject(doc.get_object());
            operation.instrumentLogo = logosStorage->getLogo(operation.instrumentId);
        }
        catch (...)
        {
            qWarning() << "Failed to parse operation";
        }
    }
}

QList<Operation> OperationsDatabase::readOperations(int partId)
{
    qDebug() << "Reading operations from database";

    QList<Operation> res;

    const std::shared_ptr<IFile> operationsFile =
        mFileFactory->newInstance(QString("%1/%2").arg(operationsDirPath(), fileName(partId)));

    if (operationsFile->open(QIODevice::ReadOnly))
    {
        QByteArray content = operationsFile->readAll();
        operationsFile->close();

        if (content.size() > MIN_FILE_SIZE_PARALLEL)
        {
            QList<int> indecies;

            FindOperationsIndeciesInfo findOperationsIndeciesInfo(content);
            processInParallel(QThread::currentThread(), indecies, findOperationsIndeciesForParallel, &findOperationsIndeciesInfo);

            MergeOperationsIndeciesInfo mergeOperationsIndeciesInfo(findOperationsIndeciesInfo.results);
            indecies.resizeForOverwrite(mergeOperationsIndeciesInfo.indecies.constLast() + 1);
            processInParallel(
                QThread::currentThread(), indecies, mergeOperationsIndeciesForParallel, &mergeOperationsIndeciesInfo
            );

            indecies[indecies.size() - 1] = content.size() - 1;

            res.resizeForOverwrite(indecies.size());

            mLogosStorage->readLock();

            ReadOperationsInfo readOperationsInfo(mLogosStorage, content, indecies);
            processInParallel(QThread::currentThread(), res, readOperationsForParallel, &readOperationsInfo);

            mLogosStorage->readUnlock();
        }
        else
        {
            content = "[" + content + "]";
            const simdjson::padded_string jsonData(content.toStdString());

            simdjson::ondemand::parser parser;

            try
            {
                simdjson::ondemand::document doc = parser.iterate(jsonData);

                simdjson::ondemand::array jsonOperations = doc.get_array();
                res.resizeForOverwrite(jsonOperations.count_elements());

                int i = res.size() - 1;

                mLogosStorage->readLock();

                for (const simdjson::ondemand::object jsonObject : jsonOperations)
                {
                    Operation& operation = res[i];

                    operation.fromJsonObject(jsonObject);
                    operation.instrumentLogo = mLogosStorage->getLogo(operation.instrumentId);

                    --i;
                }

                mLogosStorage->readUnlock();
            }
            catch (...)
            {
                qWarning() << "Failed to parse operations";
            }
        }
    }

    return res;
}

struct WriteOperationsInfo
{
    explicit WriteOperationsInfo()
    {
        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    QList<QByteArray> results;
    QByteArray*       resultsArray;
};

static void writeOperationsForParallel(
    QThread* parentThread, int threadId, Operation* operations, int /*size*/, int start, int end, void* additionalArgs
)
{
    const WriteOperationsInfo* writeOperationsInfo = reinterpret_cast<WriteOperationsInfo*>(additionalArgs);

    QByteArray* resultsArray = writeOperationsInfo->resultsArray;

    for (int i = end - 1; i >= start && !parentThread->isInterruptionRequested(); --i)
    {
        const QJsonDocument jsonDoc(operations[i].toJsonObject());

        resultsArray[threadId].append(jsonDoc.toJson(QJsonDocument::Compact));

        if (i > 0)
        {
            resultsArray[threadId].append(",\n");
        }
    }
}

void OperationsDatabase::writeOperations(QList<Operation>& operations, int partId)
{
    qDebug() << "Writing operations to database";

    const QString dirPath = operationsDirPath();

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(dirPath);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    const std::shared_ptr<IFile> operationsFile = mFileFactory->newInstance(QString("%1/%2").arg(dirPath, fileName(partId)));

    ok = operationsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    WriteOperationsInfo writeOperationsInfo;
    processInParallel(QThread::currentThread(), operations, writeOperationsForParallel, &writeOperationsInfo);

    for (int i = writeOperationsInfo.results.size() - 1; i >= 0; --i)
    {
        operationsFile->write(writeOperationsInfo.resultsArray[i]);
    }

    operationsFile->close();
}

void OperationsDatabase::appendOperations(const QList<Operation>& operations, int partId)
{
    qDebug() << "Appending operations to database";

    const std::shared_ptr<IFile> operationsFile =
        mFileFactory->newInstance(QString("%1/%2").arg(operationsDirPath(), fileName(partId)));

    const bool ok = operationsFile->open(QIODevice::Append);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    for (int i = 0; i < operations.size(); ++i)
    {
        const QJsonDocument jsonDoc(operations.at(operations.size() - i - 1).toJsonObject());

        operationsFile->write(",\n");
        operationsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    }

    operationsFile->close();
}

void OperationsDatabase::deleteOperations(int partId)
{
    qDebug() << "Deleting operations";

    const std::shared_ptr<IFile> operationsFile =
        mFileFactory->newInstance(QString("%1/%2").arg(operationsDirPath(), fileName(partId)));

    operationsFile->remove();
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

QString OperationsDatabase::fileName(int partId) const
{
    return partId < 0 ? "operations.json" : QString("operations%1.json").arg(partId);
}
