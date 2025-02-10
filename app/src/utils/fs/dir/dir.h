#pragma once



#include "src/utils/fs/dir/idir.h"

#include <QDir>



class Dir : public IDir
{
public:
    Dir(const QString& path = QString());
    ~Dir();

    Dir(const Dir& another)            = delete;
    Dir& operator=(const Dir& another) = delete;

    bool mkpath(const QString& dirPath) override;

private:
    QDir mDir;
};
