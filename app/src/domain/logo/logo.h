#pragma once



#include <QMutex>
#include <QPixmap>



struct Logo
{
    Logo()  = default;
    ~Logo() = default;

    Logo(const Logo& another)            = delete;
    Logo& operator=(const Logo& another) = delete;

    QMutex  mutex;
    QPixmap pixmap;
};

using Logos = QMap<QString, Logo*>; // Instrument UID => Logo
