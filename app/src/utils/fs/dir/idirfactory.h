#pragma once



#include "src/utils/fs/dir/idir.h"



class IDirFactory
{
public:
    IDirFactory()
    {
    }
    virtual ~IDirFactory() = default;

    IDirFactory(const IDirFactory& another)            = delete;
    IDirFactory& operator=(const IDirFactory& another) = delete;

    virtual std::shared_ptr<IDir> newInstance(const QString& path = QString()) = 0;
};
