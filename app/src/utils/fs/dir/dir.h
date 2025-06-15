#pragma once



#include "src/utils/fs/dir/idir.h"

#include <QDir>



class Dir : public IDir
{
public:
    explicit Dir(const QString& path = QString());
    ~Dir() override;

    Dir(const Dir& another)            = delete;
    Dir& operator=(const Dir& another) = delete;

    bool mkpath(const QString& dirPath) override;
    bool removeRecursively() override;
    QStringList entryList(
        const QStringList& nameFilters, QDir::Filters filters = QDir::NoFilter, QDir::SortFlags sort = QDir::NoSort
    ) override;

private:
    QDir mDir;
};
