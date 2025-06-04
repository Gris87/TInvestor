#include "src/db/logs/logsdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>



LogsDatabase::LogsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory, bool autoPilotMode) :
    ILogsDatabase(),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory),
    mAutoPilotMode(autoPilotMode),
    mAccountHash()
{
    qDebug() << "Create LogsDatabase";
}

LogsDatabase::~LogsDatabase()
{
    qDebug() << "Destroy LogsDatabase";
}

void LogsDatabase::setAccount(const QString& account)
{
    mAccountHash = account;
}

QList<LogEntry> LogsDatabase::readLogs()
{
    qDebug() << "Reading logs from database";

    QList<LogEntry> res;

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(logsDirPath() + "/logs.json");

    if (logsFile->open(QIODevice::ReadOnly))
    {
        QByteArray content  = "[";
        content            += logsFile->readAll();
        content            += "]";

        logsFile->close();

        QJsonParseError     parseError;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(content, &parseError);

        if (parseError.error == QJsonParseError::NoError)
        {
            const QJsonArray jsonLogs = jsonDoc.array();

            res.resizeForOverwrite(jsonLogs.size());

            for (int i = 0; i < jsonLogs.size(); ++i)
            {
                res[i].fromJsonObject(jsonLogs.at(i).toObject());
            }
        }
    }

    return res;
}

void LogsDatabase::appendLog(const LogEntry& entry)
{
    qDebug() << "Appending logs to database";

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(logsDirPath() + "/logs.json");

    const bool ok = logsFile->open(QIODevice::WriteOnly | QIODevice::Append);
    Q_ASSERT_X(ok, "LogsDatabase::appendLog()", "Failed to open file");

    const QJsonDocument jsonDoc(entry.toJsonObject());

    if (logsFile->size() > 0)
    {
        logsFile->write(",\n");
    }

    logsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    logsFile->close();
}

void LogsDatabase::setAutoPilotMode(bool autoPilotMode)
{
    mAutoPilotMode = autoPilotMode;
}

QString LogsDatabase::logsDirPath() const
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
