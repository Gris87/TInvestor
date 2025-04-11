#include "src/widgets/tableitems/actions/actionstableitemwidget.h"
#include "ui_actionstableitemwidget.h"

#include <QDebug>



ActionsTableItemWidget::ActionsTableItemWidget(
    IOrderWavesDialogFactory* orderWavesDialogFactory,
    IOrderWavesWidgetFactory* orderWavesWidgetFactory,
    IOrderBookThread*         orderBookThread,
    IHttpClient*              httpClient,
    Stock*                    stock,
    int                       precision,
    QWidget*                  parent
) :
    IActionsTableItemWidget(parent),
    ui(new Ui::ActionsTableItemWidget),
    mOrderWavesDialogFactory(orderWavesDialogFactory),
    mOrderWavesWidgetFactory(orderWavesWidgetFactory),
    mOrderBookThread(orderBookThread),
    mHttpClient(httpClient),
    mStock(stock),
    mPrecision(precision)
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
    const std::shared_ptr<IOrderWavesDialog> dialog =
        mOrderWavesDialogFactory->newInstance(mOrderWavesWidgetFactory, mOrderBookThread, mStock, mPrecision, this);
    dialog->exec();
}

void ActionsTableItemWidget::on_linkButton_clicked()
{
    mStock->mutex->lock();
    const QUrl url(QString("https://www.tbank.ru/invest/stocks/%1/").arg(mStock->meta.ticker));
    mStock->mutex->unlock();

    const bool ok = mHttpClient->openInBrowser(url);
    Q_ASSERT_X(ok, "ActionsTableItemWidget::on_linkButton_clicked()", "Failed to open link");
}
