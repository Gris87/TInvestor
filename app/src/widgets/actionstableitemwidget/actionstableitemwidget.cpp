#include "src/widgets/actionstableitemwidget/actionstableitemwidget.h"
#include "ui_actionstableitemwidget.h"

#include <QDebug>

#include "src/widgets/tablemodels/modelroles.h"



ActionsTableItemWidget::ActionsTableItemWidget(
    IOrderWavesDialogFactory* orderWavesDialogFactory,
    IOrderWavesWidgetFactory* orderWavesWidgetFactory,
    IOrderBookThread*         orderBookThread,
    IHttpClient*              httpClient,
    IStocksTableModel*        stocksTableModel,
    int                       tableRow,
    QWidget*                  parent
) :
    IActionsTableItemWidget(parent),
    ui(new Ui::ActionsTableItemWidget),
    mOrderWavesDialogFactory(orderWavesDialogFactory),
    mOrderWavesWidgetFactory(orderWavesWidgetFactory),
    mOrderBookThread(orderBookThread),
    mHttpClient(httpClient),
    mStocksTableModel(stocksTableModel),
    mTableRow(tableRow)
{
    qDebug() << "Create ActionsTableItemWidget";

    ui->setupUi(this);
}

ActionsTableItemWidget::~ActionsTableItemWidget()
{
    qDebug() << "Destroy ActionsTableItemWidget";

    delete ui;
}

void ActionsTableItemWidget::on_orderWavesButton_clicked()
{
    const qint64 stockAddress = mStocksTableModel->index(mTableRow, STOCKS_NAME_COLUMN).data(ROLE_STOCK).toLongLong();
    Stock*       stock        = reinterpret_cast<Stock*>(stockAddress); // NOLINT(performance-no-int-to-ptr)

    const std::shared_ptr<IOrderWavesDialog> dialog =
        mOrderWavesDialogFactory->newInstance(mOrderWavesWidgetFactory, mOrderBookThread, stock, this);
    dialog->exec();
}

void ActionsTableItemWidget::on_linkButton_clicked()
{
    const QString ticker = mStocksTableModel->index(mTableRow, STOCKS_NAME_COLUMN).data().toString();
    const QUrl    url(QString("https://www.tbank.ru/invest/stocks/%1/").arg(ticker));

    const bool ok = mHttpClient->openInBrowser(url);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open link");
}
