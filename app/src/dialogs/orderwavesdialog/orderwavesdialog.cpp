#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
constexpr float   HUNDRED_PERCENT = 100.0f;



OrderWavesDialog::OrderWavesDialog(
    IOrderWavesWidgetFactory* orderWavesWidgetFactory, IOrderBookThread* orderBookThread, Stock* stock, QWidget* parent
) :
    IOrderWavesDialog(parent),
    ui(new Ui::OrderWavesDialog),
    mOrderBookThread(orderBookThread),
    mStock(stock)
{
    qDebug() << "Create OrderWavesDialog";

    ui->setupUi(this);

    setWindowFlags(
        Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint |
        Qt::WindowCloseButtonHint
    );
    setWindowModality(Qt::ApplicationModal);

    ui->nameLabel->setText(mStock->meta.instrumentName);

    mOrderWavesWidget = orderWavesWidgetFactory->newInstance(mStock->meta.pricePrecision, mStock->meta.minPriceIncrement, this);
    mOrderWavesWidget->hide();
    ui->layoutForOrderWaves->addWidget(mOrderWavesWidget);

    connect(mOrderBookThread, SIGNAL(orderBookChanged(const OrderBook&)), this, SLOT(orderBookChanged(const OrderBook&)));

    mOrderBookThread->setStock(mStock);
    mOrderBookThread->start();
}

OrderWavesDialog::~OrderWavesDialog()
{
    qDebug() << "Destroy OrderWavesDialog";

    mOrderBookThread->terminateThread();
    mOrderBookThread->wait();

    delete ui;
}

void OrderWavesDialog::orderBookChanged(const OrderBook& orderBook)
{
    const QString price = QString::number(orderBook.price, 'f', mStock->meta.pricePrecision) + " \u20BD";
    QString       spread;

    if (!orderBook.bids.isEmpty() && !orderBook.asks.isEmpty())
    {
        const float spreadPrice   = orderBook.asks.constFirst().price - orderBook.bids.constFirst().price;
        const float spreadPercent = (orderBook.asks.constFirst().price / orderBook.bids.constFirst().price - 1) * HUNDRED_PERCENT;

        spread = QString("%1 \u20BD (%2%)")
                     .arg(QString::number(spreadPrice, 'f', mStock->meta.pricePrecision), QString::number(spreadPercent, 'f', 3));
    }
    else
    {
        spread = "-";
    }

    ui->timeLabel->setText(QDateTime::fromMSecsSinceEpoch(orderBook.timestamp).toString(DATETIME_FORMAT));
    ui->priceLabel->setText(tr("%1 / Spread: %2").arg(price, spread));

    mOrderWavesWidget->orderBookChanged(orderBook);

    mOrderWavesWidget->show();
}

void OrderWavesDialog::on_resetButton_clicked()
{
    mOrderWavesWidget->reset();
}
