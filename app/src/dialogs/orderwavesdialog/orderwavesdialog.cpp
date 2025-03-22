#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <QDebug>



OrderWavesDialog::OrderWavesDialog(
    IOrderWavesWidgetFactory* orderWavesWidgetFactory,
    IOrderBookThread*         orderBookThread,
    Stock*                    stock,
    int                       precision,
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
        Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint |
        Qt::WindowCloseButtonHint
    );

    ui->nameLabel->setText(mStock->meta.name);

    mOrderWavesWidget = orderWavesWidgetFactory->newInstance(this);
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

void OrderWavesDialog::resizeEvent(QResizeEvent* event)
{
    IOrderWavesDialog::resizeEvent(event);

    QRect windowRect(0, 0, width(), height());

    ui->layoutForOrderWavesWidget->setGeometry(windowRect);
    ui->layoutForControlsWidget->setGeometry(windowRect);
}

void OrderWavesDialog::orderBookChanged(const OrderBook& orderBook)
{
    ui->timeLabel->setText(QDateTime::fromMSecsSinceEpoch(orderBook.timestamp).toString("yyyy-MM-dd hh:mm:ss.zzz"));
    ui->priceLabel->setText(QString::number(orderBook.price, 'f', mPrecision) + " " + QChar(0x20BD));
}

void OrderWavesDialog::on_resetButton_clicked()
{
}
