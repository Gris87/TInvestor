#pragma once



#include "src/widgets/trayicon/itrayiconfactory.h"



class TrayIconFactory : public ITrayIconFactory
{
public:
    TrayIconFactory();
    ~TrayIconFactory();

    TrayIconFactory(const TrayIconFactory &another) = delete;
    TrayIconFactory& operator=(const TrayIconFactory &another) = delete;

    ITrayIcon* newInstance(QWidget *parent) override;
};
