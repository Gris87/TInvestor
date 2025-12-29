#pragma once



#include "src/domain/bidirinfo/bidirinfo.h"
#include "src/utils/fs/file/ifile.h"



class IBiDirInfosDatabase
{
public:
    explicit IBiDirInfosDatabase() = default;
    virtual ~IBiDirInfosDatabase() = default;

    IBiDirInfosDatabase(const IBiDirInfosDatabase& another)            = delete;
    IBiDirInfosDatabase& operator=(const IBiDirInfosDatabase& another) = delete;

    virtual BiDirInfos readBiDirInfos(std::shared_ptr<IFile> biDirInfoFile) = 0;
};
