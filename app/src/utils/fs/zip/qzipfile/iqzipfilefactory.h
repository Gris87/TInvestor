#pragma once



#include <QString>

#include "src/utils/fs/zip/qzipfile/iqzipfile.h"



class IQZipFileFactory
{
public:
    IQZipFileFactory()          = default;
    virtual ~IQZipFileFactory() = default;

    IQZipFileFactory(const IQZipFileFactory& another)            = delete;
    IQZipFileFactory& operator=(const IQZipFileFactory& another) = delete;

    virtual std::shared_ptr<IQZipFile> newInstance(QuaZip* zip) const = 0;
};
