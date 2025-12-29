#pragma once



#include "src/db/bidirinfos/ibidirinfosdatabase.h"

#include <gmock/gmock.h>



class BiDirInfosDatabaseMock : public IBiDirInfosDatabase
{
public:
    explicit BiDirInfosDatabaseMock() :
        IBiDirInfosDatabase()
    {
    }
    ~BiDirInfosDatabaseMock() override = default;

    BiDirInfosDatabaseMock(const BiDirInfosDatabaseMock& another)            = delete;
    BiDirInfosDatabaseMock& operator=(const BiDirInfosDatabaseMock& another) = delete;

    MOCK_METHOD(BiDirInfos, readBiDirInfos, (std::shared_ptr<IFile> biDirInfoFile), (override));
};
