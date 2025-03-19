#pragma once



#include "src/dialogs/marketwavesdialog/imarketwavesdialog.h"

#include <gmock/gmock.h>



class MarketWavesDialogMock : public IMarketWavesDialog
{
public:
    explicit MarketWavesDialogMock(QWidget* parent = nullptr) :
        IMarketWavesDialog(parent)
    {
    }
    ~MarketWavesDialogMock() override = default;

    MarketWavesDialogMock(const MarketWavesDialogMock& another)            = delete;
    MarketWavesDialogMock& operator=(const MarketWavesDialogMock& another) = delete;

    MOCK_METHOD(int, exec, (), (override));
};
