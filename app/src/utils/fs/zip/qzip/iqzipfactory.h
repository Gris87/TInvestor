#pragma once



#include "src/utils/fs/zip/qzip/iqzip.h"

#include <QString>



class IQZipFactory
{
public:
    IQZipFactory()          = default;
    virtual ~IQZipFactory() = default;

    IQZipFactory(const IQZipFactory& another)            = delete;
    IQZipFactory& operator=(const IQZipFactory& another) = delete;

    virtual std::shared_ptr<IQZip> newInstance(const QString& name) const = 0;
};
