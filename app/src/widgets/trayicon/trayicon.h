#pragma once



#include "src/widgets/trayicon/itrayicon.h"



class TrayIcon : public ITrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject* parent = nullptr);
    ~TrayIcon();

    TrayIcon(const TrayIcon& another)            = delete;
    TrayIcon& operator=(const TrayIcon& another) = delete;
};
