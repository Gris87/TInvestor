#pragma once



#include "src/utils/fs/file/ifilefactory.h"



class FileFactory : public IFileFactory
{
public:
    FileFactory();
    ~FileFactory();

    FileFactory(const FileFactory& another)            = delete;
    FileFactory& operator=(const FileFactory& another) = delete;

    IFile* newInstance(const QString& name) override;
};
