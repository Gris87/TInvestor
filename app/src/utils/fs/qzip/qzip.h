#pragma once



#include "src/utils/fs/qzip/iqzip.h"

#include <QString>



class QZip : public IQZip
{
public:
    QZip(const QString& name);
    ~QZip();

    QZip(const QZip& another)            = delete;
    QZip& operator=(const QZip& another) = delete;
};
