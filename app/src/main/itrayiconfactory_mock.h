#pragma once



#include "src/main/itrayiconfactory.h"

#include <gmock/gmock.h>



class TrayIconFactoryMock : public ITrayIconFactory
{
public:
    TrayIconFactoryMock() {}
    ~TrayIconFactoryMock() override = default;

    TrayIconFactoryMock(const TrayIconFactoryMock &another) = delete;
    TrayIconFactoryMock& operator=(const TrayIconFactoryMock &another) = delete;

    MOCK_METHOD1(newInstance, ITrayIcon*(QWidget *parent));
};
