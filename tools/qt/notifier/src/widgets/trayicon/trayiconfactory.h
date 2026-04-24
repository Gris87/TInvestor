#pragma once



#include "src/widgets/trayicon/itrayiconfactory.h"



class TrayIconFactory : public ITrayIconFactory
{
public:
    TrayIconFactory();
    ~TrayIconFactory() override;

    TrayIconFactory(const TrayIconFactory& another)            = delete;
    TrayIconFactory& operator=(const TrayIconFactory& another) = delete;

    ITrayIcon* newInstance(QObject* parent) const override;
};
