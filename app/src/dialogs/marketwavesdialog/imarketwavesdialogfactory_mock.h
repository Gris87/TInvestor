#pragma once



#include "src/dialogs/marketwavesdialog/imarketwavesdialogfactory.h"

#include <gmock/gmock.h>



class MarketWavesDialogFactoryMock : public IMarketWavesDialogFactory
{
public:
    MarketWavesDialogFactoryMock() :
        IMarketWavesDialogFactory()
    {
    }
    ~MarketWavesDialogFactoryMock() override = default;

    MarketWavesDialogFactoryMock(const MarketWavesDialogFactoryMock& another)            = delete;
    MarketWavesDialogFactoryMock& operator=(const MarketWavesDialogFactoryMock& another) = delete;

    MOCK_METHOD(
        std::shared_ptr<IMarketWavesDialog>,
        newInstance,
        (IMarketWavesThread * marketWavesThread, Stock* stock, QWidget* parent),
        (override)
    );
};
