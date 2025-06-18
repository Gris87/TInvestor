#pragma once



#include <QMutex>
#include <QPixmap>



struct Logo
{
    Logo()                    = default;
    Logo(const Logo& another) = default;
    ~Logo()                   = default;

    Logo& operator=(const Logo& another) = default;

    QMutex  mutex;
    QPixmap pixmap;
};

using Logos = QMap<QString, Logo*>; // Instrument UID => Logo
