#pragma once



#include "src/widgets/tablerecord/items/actions/iactionstableitemwidget.h"

#include "src/dialogs/marketwavesdialog/imarketwavesdialogfactory.h"
#include "src/domain/stocks/stock.h"
#include "src/threads/marketwaves/imarketwavesthread.h"
#include "src/utils/http/ihttpclient.h"



namespace Ui
{
class ActionsTableItemWidget;
}



class ActionsTableItemWidget : public IActionsTableItemWidget
{
    Q_OBJECT

public:
    explicit ActionsTableItemWidget(
        IMarketWavesDialogFactory* marketWavesDialogFactory,
        IMarketWavesThread*        marketWavesThread,
        IHttpClient*               httpClient,
        Stock*                     stock,
        QWidget*                   parent = nullptr
    );
    ~ActionsTableItemWidget();

    ActionsTableItemWidget(const ActionsTableItemWidget& another)            = delete;
    ActionsTableItemWidget& operator=(const ActionsTableItemWidget& another) = delete;

    Ui::ActionsTableItemWidget* ui;

private:
    IMarketWavesDialogFactory* mMarketWavesDialogFactory;
    IMarketWavesThread*        mMarketWavesThread;
    IHttpClient*               mHttpClient;
    Stock*                     mStock;

private slots:
    void on_marketWavesButton_clicked();
    void on_linkButton_clicked();
};
