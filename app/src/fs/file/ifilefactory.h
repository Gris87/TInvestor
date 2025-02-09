#pragma once



#include "src/fs/file/ifile.h"



class IFileFactory
{
public:
    IFileFactory()
    {
    }
    virtual ~IFileFactory() = default;

    IFileFactory(const IFileFactory& another)            = delete;
    IFileFactory& operator=(const IFileFactory& another) = delete;

    virtual IFile* newInstance() = 0;
};
