#pragma once



#include "src/utils/fs/file/ifilefactory.h"



class FileFactory : public IFileFactory
{
public:
    FileFactory();
    ~FileFactory() override;

    FileFactory(const FileFactory& another)            = delete;
    FileFactory& operator=(const FileFactory& another) = delete;

    [[nodiscard]]
    std::shared_ptr<IFile> newInstance(const QString& name) const override;
};
