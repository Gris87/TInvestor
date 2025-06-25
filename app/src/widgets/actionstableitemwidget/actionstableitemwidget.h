#pragma once



#include "src/widgets/actionstableitemwidget/iactionstableitemwidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel.h"



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
        IStocksTableModel*        stocksTableModel,
        int                       tableRow,
        QWidget*                  parent = nullptr
    );
    ~ActionsTableItemWidget() override;

    ActionsTableItemWidget(const ActionsTableItemWidget& another)            = delete;
    ActionsTableItemWidget& operator=(const ActionsTableItemWidget& another) = delete;

    Ui::ActionsTableItemWidget* ui;

private:
    IOrderWavesDialogFactory* mOrderWavesDialogFactory;
    IOrderWavesWidgetFactory* mOrderWavesWidgetFactory;
    IOrderBookThread*         mOrderBookThread;
    IHttpClient*              mHttpClient;
    IStocksTableModel*        mStocksTableModel;
    int                       mTableRow;

private slots:
    void on_orderWavesButton_clicked();
    void on_linkButton_clicked();
};
