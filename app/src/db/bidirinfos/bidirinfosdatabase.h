#pragma once



#include "src/db/bidirinfos/ibidirinfosdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"



class BiDirInfosDatabase : public IBiDirInfosDatabase
{
public:
    explicit BiDirInfosDatabase(IDirFactory* dirFactory);
    ~BiDirInfosDatabase() override;

    BiDirInfosDatabase(const BiDirInfosDatabase& another)            = delete;
    BiDirInfosDatabase& operator=(const BiDirInfosDatabase& another) = delete;

    BiDirInfos readBiDirInfos(std::shared_ptr<IFile> biDirInfoFile) override;
};
