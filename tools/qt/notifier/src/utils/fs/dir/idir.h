#pragma once



#include <QDir>



class IDir
{
public:
    IDir()          = default;
    virtual ~IDir() = default;

    IDir(const IDir& another)            = delete;
    IDir& operator=(const IDir& another) = delete;

    virtual bool mkpath(const QString& dirPath) = 0;
    virtual bool removeRecursively()            = 0;
    virtual QStringList
    entryList(const QStringList& nameFilters, QDir::Filters filters = QDir::NoFilter, QDir::SortFlags sort = QDir::NoSort) = 0;
};
