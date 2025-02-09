#pragma once



#include "src/fs/dir/idir.h"

#include <gmock/gmock.h>



class DirMock : public IDir
{
public:
    DirMock() :
        IDir()
    {
    }
    ~DirMock() override = default;

    DirMock(const DirMock& another)            = delete;
    DirMock& operator=(const DirMock& another) = delete;
};
