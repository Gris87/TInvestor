#pragma once



#include "src/widgets/trayicon/itrayicon.h"



class ITrayIconFactory
{
public:
    ITrayIconFactory()          = default;
    virtual ~ITrayIconFactory() = default;

    ITrayIconFactory(const ITrayIconFactory& another)            = delete;
    ITrayIconFactory& operator=(const ITrayIconFactory& another) = delete;

    virtual ITrayIcon* newInstance(QObject* parent) const = 0;
};
