#pragma once



#include "src/main/itrayicon.h"



class TrayIcon : public ITrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QWidget *parent = nullptr);
    ~TrayIcon();

    TrayIcon(const TrayIcon &another) = delete;
    TrayIcon& operator=(const TrayIcon &another) = delete;
};
