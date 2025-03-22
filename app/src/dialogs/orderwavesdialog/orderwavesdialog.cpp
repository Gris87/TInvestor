#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <QDebug>



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
        Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint |
        Qt::WindowCloseButtonHint
    );

    mOrderWavesWidget = orderWavesWidgetFactory->newInstance(this);
    ui->layoutForOrderWavesWidget->addWidget(mOrderWavesWidget);

    connect(mOrderBookThread, SIGNAL(orderBookChanged(const OrderBook&)), this, SLOT(orderBookChanged(const OrderBook&)));

    mOrderBookThread->setStock(mStock);
    mOrderBookThread->start();
}

OrderWavesDialog::~OrderWavesDialog()
{
    qDebug() << "Destroy OrderWavesDialog";

    mOrderBookThread->terminateThread();

    delete ui;
}

void OrderWavesDialog::orderBookChanged(const OrderBook& orderBook)
{
    ui->timeLabel->setText(QDateTime::fromMSecsSinceEpoch(orderBook.timestamp).toString());

    qInfo() << "==============";
    qInfo() << orderBook.timestamp;

    for (int i = 0; i < orderBook.bids.size(); ++i)
    {
        qInfo() << orderBook.bids.at(i).quantity << "-" << orderBook.bids.at(i).price;
    }

    for (int i = 0; i < orderBook.asks.size(); ++i)
    {
        qInfo() << orderBook.asks.at(i).quantity << "-" << orderBook.asks.at(i).price;
    }
}

void OrderWavesDialog::on_resetButton_clicked()
{
}
