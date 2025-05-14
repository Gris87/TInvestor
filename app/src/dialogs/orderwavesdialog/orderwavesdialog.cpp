#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <QDebug>

#include "src/grpc/utils.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
constexpr QChar   RUBLE           = QChar(0x20BD);



OrderWavesDialog::OrderWavesDialog(
    IOrderWavesWidgetFactory* orderWavesWidgetFactory,
    IOrderBookThread*         orderBookThread,
    Stock*                    stock,
    qint8                     precision,
    QWidget*                  parent
) :
    IOrderWavesDialog(parent),
    ui(new Ui::OrderWavesDialog),
    mOrderBookThread(orderBookThread),
    mStock(stock),
    mPrecision(precision)
{
    qDebug() << "Create OrderWavesDialog";

    ui->setupUi(this);

    setWindowFlags(
        Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint |
        Qt::WindowCloseButtonHint
    );
    setWindowModality(Qt::ApplicationModal);

    ui->nameLabel->setText(mStock->meta.name);

    mOrderWavesWidget = orderWavesWidgetFactory->newInstance(mPrecision, quotationToFloat(mStock->meta.minPriceIncrement), this);
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
    ui->timeLabel->setText(QDateTime::fromMSecsSinceEpoch(orderBook.timestamp).toString(DATETIME_FORMAT));
    ui->priceLabel->setText(QString::number(orderBook.price, 'f', mPrecision) + " " + RUBLE);

    mOrderWavesWidget->orderBookChanged(orderBook);
}

void OrderWavesDialog::on_resetButton_clicked()
{
    mOrderWavesWidget->reset();
}
