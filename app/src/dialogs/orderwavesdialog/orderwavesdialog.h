#pragma once



#include "src/dialogs/orderwavesdialog/iorderwavesdialog.h"

#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"



namespace Ui
{
class OrderWavesDialog;
}



class OrderWavesDialog : public IOrderWavesDialog
{
    Q_OBJECT

public:
    explicit OrderWavesDialog(
        IOrderWavesWidgetFactory* orderWavesWidgetFactory,
        IOrderBookThread*         orderBookThread,
        Stock*                    stock,
        int                       precision,
        QWidget*                  parent = nullptr
    );
    ~OrderWavesDialog();

    OrderWavesDialog(const OrderWavesDialog& another)            = delete;
    OrderWavesDialog& operator=(const OrderWavesDialog& another) = delete;

    Ui::OrderWavesDialog* ui;

private:
    IOrderWavesWidget* mOrderWavesWidget;
    IOrderBookThread*  mOrderBookThread;
    Stock*            mStock;
    int                mPrecision;

public slots:
    void orderBookChanged(const OrderBook& orderBook);

private slots:
    void on_resetButton_clicked();
};
