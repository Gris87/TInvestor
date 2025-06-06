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

private:
    QDir mDir;
};
