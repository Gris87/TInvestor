#pragma once



#include "src/main/itrayicon.h"

#include <gmock/gmock.h>



class TrayIconMock : public ITrayIcon
{
public:
    explicit TrayIconMock(QWidget *parent = nullptr) : ITrayIcon(parent) {}
    ~TrayIconMock() override = default;

    TrayIconMock(const TrayIconMock &another) = delete;
    TrayIconMock& operator=(const TrayIconMock &another) = delete;
};
