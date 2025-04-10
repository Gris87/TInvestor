#pragma once



#include "src/utils/fs/zip/qzipfile/iqzipfile.h"

#include <QString>



class IQZipFileFactory
{
public:
    IQZipFileFactory()          = default;
    virtual ~IQZipFileFactory() = default;

    IQZipFileFactory(const IQZipFileFactory& another)            = delete;
    IQZipFileFactory& operator=(const IQZipFileFactory& another) = delete;

    virtual std::shared_ptr<IQZipFile> newInstance(QuaZip* zip) const = 0;
};
