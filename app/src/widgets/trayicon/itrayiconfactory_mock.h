#pragma once



#include "src/widgets/trayicon/itrayiconfactory.h"

#include <gmock/gmock.h>



class TrayIconFactoryMock : public ITrayIconFactory
{
public:
    TrayIconFactoryMock() : ITrayIconFactory() {}
    ~TrayIconFactoryMock() override = default;

    TrayIconFactoryMock(const TrayIconFactoryMock &another) = delete;
    TrayIconFactoryMock& operator=(const TrayIconFactoryMock &another) = delete;

    MOCK_METHOD1(newInstance, ITrayIcon*(QWidget *parent));
};
