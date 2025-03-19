#include "src/widgets/tablerecord/items/actions/actionstableitemwidget.h"
#include "ui_actionstableitemwidget.h"

#include <QDebug>



ActionsTableItemWidget::ActionsTableItemWidget(
    IMarketWavesDialogFactory* marketWavesDialogFactory,
    IMarketWavesThread*        marketWavesThread,
    IHttpClient*               httpClient,
    Stock*                     stock,
    QWidget*                   parent
) :
    IActionsTableItemWidget(parent),
    ui(new Ui::ActionsTableItemWidget),
    mMarketWavesDialogFactory(marketWavesDialogFactory),
    mMarketWavesThread(marketWavesThread),
    mHttpClient(httpClient),
    mStock(stock)
{
    qDebug() << "Create ActionsTableItemWidget";

    ui->setupUi(this);
}

ActionsTableItemWidget::~ActionsTableItemWidget()
{
    qDebug() << "Destroy ActionsTableItemWidget";

    delete ui;
}

void ActionsTableItemWidget::on_marketWavesButton_clicked()
{
    std::shared_ptr<IMarketWavesDialog> dialog = mMarketWavesDialogFactory->newInstance(mMarketWavesThread, mStock, this);
    dialog->exec();
}

void ActionsTableItemWidget::on_linkButton_clicked()
{
    mStock->mutex->lock();
    QUrl url(QString("https://www.tbank.ru/invest/stocks/%1/").arg(mStock->meta.ticker));
    mStock->mutex->unlock();

    bool ok = mHttpClient->openInBrowser(url);
    Q_ASSERT_X(ok, "ActionsTableItemWidget::on_linkButton_clicked()", "Failed to open link");
}
