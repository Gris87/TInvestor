#pragma once



#include "src/fs/file/ifilefactory.h"



class FileFactory : public IFileFactory
{
public:
    FileFactory();
    ~FileFactory();

    FileFactory(const FileFactory& another)            = delete;
    FileFactory& operator=(const FileFactory& another) = delete;

    IFile* newInstance() override;
};
