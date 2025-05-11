#pragma once



#include "src/db/operations/ioperationsdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class OperationsDatabase : public IOperationsDatabase
{
public:
    explicit OperationsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~OperationsDatabase() override;

    OperationsDatabase(const OperationsDatabase& another)            = delete;
    OperationsDatabase& operator=(const OperationsDatabase& another) = delete;

    QList<Operation> readOperations() override;
    void             writeOperations(const QList<Operation>& operations) override;
    void             appendOperations(const QList<Operation>& operations) override;

private:
    IDirFactory*  mDirFactory;
    IFileFactory* mFileFactory;
};
