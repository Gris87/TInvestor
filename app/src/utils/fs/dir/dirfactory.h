#pragma once



#include "src/utils/fs/dir/idirfactory.h"



class DirFactory : public IDirFactory
{
public:
    DirFactory();
    ~DirFactory();

    DirFactory(const DirFactory& another)            = delete;
    DirFactory& operator=(const DirFactory& another) = delete;

    std::shared_ptr<IDir> newInstance(const QString& path = QString()) override;
};
