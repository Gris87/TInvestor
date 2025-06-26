#pragma once



#include <QString>

#include "src/utils/fs/zip/qzip/iqzip.h"



class IQZipFactory
{
public:
    IQZipFactory()          = default;
    virtual ~IQZipFactory() = default;

    IQZipFactory(const IQZipFactory& another)            = delete;
    IQZipFactory& operator=(const IQZipFactory& another) = delete;

    [[nodiscard]]
    virtual std::shared_ptr<IQZip> newInstance(QIODevice* device) const = 0;
};
