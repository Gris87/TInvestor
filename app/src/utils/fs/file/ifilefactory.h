#pragma once



#include "src/utils/fs/file/ifile.h"



class IFileFactory
{
public:
    IFileFactory()          = default;
    virtual ~IFileFactory() = default;

    IFileFactory(const IFileFactory& another)            = delete;
    IFileFactory& operator=(const IFileFactory& another) = delete;

    [[nodiscard]]
    virtual std::shared_ptr<IFile> newInstance(const QString& name) const = 0;
};
