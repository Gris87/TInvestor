#pragma once



#include <QMutex>
#include <QPixmap>



struct Logo
{
    Logo()  = default;
    ~Logo() = default;

    QMutex  mutex;
    QPixmap pixmap;
};

using Logos = QMap<QString, Logo*>; // Instrument UID => Logo
