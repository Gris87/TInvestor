#pragma once



#include "src/fs/dir/idirfactory.h"



class DirFactory : public IDirFactory
{
public:
    DirFactory();
    ~DirFactory();

    DirFactory(const DirFactory& another)            = delete;
    DirFactory& operator=(const DirFactory& another) = delete;

    IDir* newInstance(const QString& path = QString()) override;
};
