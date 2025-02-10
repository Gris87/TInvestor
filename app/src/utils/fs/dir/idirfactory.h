#pragma once



#include "src/fs/dir/idir.h"



class IDirFactory
{
public:
    IDirFactory()
    {
    }
    virtual ~IDirFactory() = default;

    IDirFactory(const IDirFactory& another)            = delete;
    IDirFactory& operator=(const IDirFactory& another) = delete;

    virtual IDir* newInstance(const QString& path = QString()) = 0;
};
