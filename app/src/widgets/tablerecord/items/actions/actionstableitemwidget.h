#pragma once



#include "src/widgets/tablerecord/items/actions/iactionstableitemwidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/domain/stocks/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"



namespace Ui
{
class ActionsTableItemWidget;
}



class ActionsTableItemWidget : public IActionsTableItemWidget
{
    Q_OBJECT

public:
    explicit ActionsTableItemWidget(
        IOrderWavesDialogFactory* orderWavesDialogFactory,
        IOrderWavesWidgetFactory* orderWavesWidgetFactory,
        IOrderBookThread*         orderBookThread,
        IHttpClient*              httpClient,
        Stock*                    stock,
        QWidget*                  parent = nullptr
    );
    ~ActionsTableItemWidget();

    ActionsTableItemWidget(const ActionsTableItemWidget& another)            = delete;
    ActionsTableItemWidget& operator=(const ActionsTableItemWidget& another) = delete;

    Ui::ActionsTableItemWidget* ui;

private:
    IOrderWavesDialogFactory* mOrderWavesDialogFactory;
    IOrderWavesWidgetFactory* mOrderWavesWidgetFactory;
    IOrderBookThread*         mOrderBookThread;
    IHttpClient*              mHttpClient;
    Stock*                    mStock;

private slots:
    void on_orderWavesButton_clicked();
    void on_linkButton_clicked();
};
