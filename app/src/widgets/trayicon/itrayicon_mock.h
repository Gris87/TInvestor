#pragma once



#include "src/widgets/trayicon/itrayicon.h"

#include <gmock/gmock.h>



class TrayIconMock : public ITrayIcon
{
    Q_OBJECT

public:
    explicit TrayIconMock(QObject* parent = nullptr) :
        ITrayIcon(parent)
    {
    }
    ~TrayIconMock() override = default;

    TrayIconMock(const TrayIconMock& another)            = delete;
    TrayIconMock& operator=(const TrayIconMock& another) = delete;
};
