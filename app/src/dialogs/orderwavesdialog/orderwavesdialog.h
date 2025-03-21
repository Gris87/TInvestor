#pragma once



#include "src/dialogs/orderwavesdialog/iorderwavesdialog.h"

#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"



namespace Ui
{
class OrderWavesDialog;
}



class OrderWavesDialog : public IOrderWavesDialog
{
    Q_OBJECT

public:
    explicit OrderWavesDialog(IOrderBookThread* orderBookThread, Stock* stock, QWidget* parent = nullptr);
    ~OrderWavesDialog();

    OrderWavesDialog(const OrderWavesDialog& another)            = delete;
    OrderWavesDialog& operator=(const OrderWavesDialog& another) = delete;

    Ui::OrderWavesDialog* ui;

private:
    IOrderBookThread* mOrderBookThread;
    Stock*            mStock;

private slots:
    void orderBookChanged(const tinkoff::OrderBook& orderBook);
};
