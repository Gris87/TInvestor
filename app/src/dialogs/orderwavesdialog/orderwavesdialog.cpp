#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <QDebug>

#include "src/grpc/utils.h"



OrderWavesDialog::OrderWavesDialog(IOrderBookThread* orderBookThread, Stock* stock, QWidget* parent) :
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

    connect(
        mOrderBookThread,
        SIGNAL(orderBookChanged(const tinkoff::OrderBook&)),
        this,
        SLOT(orderBookChanged(const tinkoff::OrderBook&))
    );

    mOrderBookThread->setStock(mStock);
    mOrderBookThread->start();
}

OrderWavesDialog::~OrderWavesDialog()
{
    qDebug() << "Destroy OrderWavesDialog";

    mOrderBookThread->terminateThread();

    delete ui;
}

void OrderWavesDialog::orderBookChanged(const tinkoff::OrderBook& orderBook)
{
    qInfo() << "==============";
    qInfo() << orderBook.figi();
    qInfo() << orderBook.depth();
    qInfo() << orderBook.is_consistent();
    qInfo() << orderBook.time().seconds();
    qInfo() << quotationToFloat(orderBook.limit_up());
    qInfo() << quotationToFloat(orderBook.limit_down());
    qInfo() << orderBook.instrument_uid();
    qInfo() << orderBook.order_book_type();
    qInfo() << orderBook.bids_size();

    for (int i = 0; i < orderBook.bids_size(); ++i)
    {
        qInfo() << orderBook.bids(i).quantity() << "-" << quotationToFloat(orderBook.bids(i).price());
    }

    qInfo() << orderBook.asks_size();

    for (int i = 0; i < orderBook.asks_size(); ++i)
    {
        qInfo() << orderBook.asks(i).quantity() << "-" << quotationToFloat(orderBook.asks(i).price());
    }
}
