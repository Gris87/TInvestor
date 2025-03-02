#pragma once



#include "src/utils/fs/qzip/iqzip.h"

#include <QString>



class IQZipFactory
{
public:
    IQZipFactory()
    {
    }
    virtual ~IQZipFactory() = default;

    IQZipFactory(const IQZipFactory& another)            = delete;
    IQZipFactory& operator=(const IQZipFactory& another) = delete;

    virtual std::shared_ptr<IQZip> newInstance(const QString& name) = 0;
};
