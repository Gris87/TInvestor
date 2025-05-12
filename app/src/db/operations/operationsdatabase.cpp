#include "src/db/operations/operationsdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>



OperationsDatabase::OperationsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory, bool autoPilotMode) :
    IOperationsDatabase(),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory),
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

QList<Operation> OperationsDatabase::readOperations()
{
    qDebug() << "Reading operations from database";

    QList<Operation> res;

    const std::shared_ptr<IFile> operationsFile = mFileFactory->newInstance(operationsDirPath() + "/operations.json");

    if (operationsFile->open(QIODevice::ReadOnly))
    {
        QByteArray content  = "[";
        content            += operationsFile->readAll();
        content            += "]";

        operationsFile->close();

        QJsonParseError     parseError;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(content, &parseError);

        if (parseError.error == QJsonParseError::NoError)
        {
            const QJsonArray jsonOperations = jsonDoc.array();

            res.resizeForOverwrite(jsonOperations.size());

            for (int i = 0; i < jsonOperations.size(); ++i)
            {
                Operation operation;

                operation.fromJsonObject(jsonOperations.at(i).toObject());

                res[i] = operation;
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
    Q_ASSERT_X(ok, "OperationsDatabase::writeOperations()", "Failed to create dir");

    const std::shared_ptr<IFile> operationsFile = mFileFactory->newInstance(dirPath + "/operations.json");

    ok = operationsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "OperationsDatabase::writeOperations()", "Failed to open file");

    for (int i = 0; i < operations.size(); ++i)
    {
        const QJsonDocument jsonDoc(operations.at(i).toJsonObject());

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
    Q_ASSERT_X(ok, "OperationsDatabase::appendOperations()", "Failed to open file");

    for (const Operation& operation : operations)
    {
        const QJsonDocument jsonDoc(operation.toJsonObject());

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
