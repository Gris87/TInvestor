#pragma once



#include "src/dialogs/marketwavesdialog/imarketwavesdialog.h"

#include "src/domain/stocks/stock.h"
#include "src/threads/marketwaves/imarketwavesthread.h"



class IMarketWavesDialogFactory
{
public:
    IMarketWavesDialogFactory()
    {
    }
    virtual ~IMarketWavesDialogFactory() = default;

    IMarketWavesDialogFactory(const IMarketWavesDialogFactory& another)            = delete;
    IMarketWavesDialogFactory& operator=(const IMarketWavesDialogFactory& another) = delete;

    virtual std::shared_ptr<IMarketWavesDialog>
    newInstance(IMarketWavesThread* marketWavesThread, Stock* stock, QWidget* parent) = 0;
};
