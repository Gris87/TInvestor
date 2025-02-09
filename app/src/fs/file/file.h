#pragma once



#include "src/fs/file/ifile.h"



class File : public IFile
{
public:
    File();
    ~File();

    File(const File& another)            = delete;
    File& operator=(const File& another) = delete;
};
