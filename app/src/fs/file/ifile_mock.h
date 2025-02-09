#pragma once



#include "src/fs/file/ifile.h"

#include <gmock/gmock.h>



class FileMock : public IFile
{
public:
    FileMock() :
        IFile()
    {
    }
    ~FileMock() override = default;

    FileMock(const FileMock& another)            = delete;
    FileMock& operator=(const FileMock& another) = delete;
};
