#pragma once



#include "src/utils/fs/dir/idir.h"



class IDirFactory
{
public:
    IDirFactory()          = default;
    virtual ~IDirFactory() = default;

    IDirFactory(const IDirFactory& another)            = delete;
    IDirFactory& operator=(const IDirFactory& another) = delete;

    [[nodiscard]]
    virtual std::shared_ptr<IDir> newInstance(const QString& path = QString()) const = 0;
};
