#pragma once



#include "src/dialogs/marketwavesdialog/imarketwavesdialog.h"

#include "src/domain/stocks/stock.h"
#include "src/threads/marketwaves/imarketwavesthread.h"



namespace Ui
{
class MarketWavesDialog;
}



class MarketWavesDialog : public IMarketWavesDialog
{
    Q_OBJECT

public:
    explicit MarketWavesDialog(IMarketWavesThread* marketWavesThread, Stock* stock, QWidget* parent = nullptr);
    ~MarketWavesDialog();

    MarketWavesDialog(const MarketWavesDialog& another)            = delete;
    MarketWavesDialog& operator=(const MarketWavesDialog& another) = delete;

    Ui::MarketWavesDialog* ui;

private:
    IMarketWavesThread* mMarketWavesThread;
    Stock*              mStock;
};
