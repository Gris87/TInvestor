#pragma once



#include "src/db/operations/ioperationsdatabase.h"

#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class OperationsDatabase : public IOperationsDatabase
{
public:
    explicit OperationsDatabase(
        IDirFactory* dirFactory, IFileFactory* fileFactory, ILogosStorage* logosStorage, bool autoPilotMode
    );
    ~OperationsDatabase() override;

    OperationsDatabase(const OperationsDatabase& another)            = delete;
    OperationsDatabase& operator=(const OperationsDatabase& another) = delete;

    void             setAccount(const QString& account) override;
    QList<Operation> readOperations() override;
    void             writeOperations(QList<Operation>& operations) override;
    void             appendOperations(const QList<Operation>& operations) override;

    void setAutoPilotMode(bool autoPilotMode);

    [[nodiscard]]
    QString operationsDirPath() const;

private:
    IDirFactory*   mDirFactory;
    IFileFactory*  mFileFactory;
    ILogosStorage* mLogosStorage;
    bool           mAutoPilotMode;
    QString        mAccountHash;
};
