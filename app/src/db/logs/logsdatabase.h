#pragma once



#include "src/db/logs/ilogsdatabase.h"

#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class LogsDatabase : public ILogsDatabase
{
public:
    explicit LogsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory, ILogosStorage* logosStorage, bool autoPilotMode);
    ~LogsDatabase() override;

    LogsDatabase(const LogsDatabase& another)            = delete;
    LogsDatabase& operator=(const LogsDatabase& another) = delete;

    void            setAccount(const QString& account) override;
    QList<LogEntry> readLogs() override;
    void            writeLogs(QList<LogEntry>& entries) override;
    void            appendLog(const LogEntry& entry) override;

    void setAutoPilotMode(bool autoPilotMode);

    [[nodiscard]]
    QString logsDirPath() const;

private:
    IDirFactory*   mDirFactory;
    IFileFactory*  mFileFactory;
    ILogosStorage* mLogosStorage;
    bool           mAutoPilotMode;
    QString        mAccountHash;
};
