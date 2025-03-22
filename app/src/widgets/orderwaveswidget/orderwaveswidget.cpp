#include "src/widgets/orderwaveswidget/orderwaveswidget.h"

#include <QDebug>

#define ORDER_WIDTH 20



OrderWavesWidget::OrderWavesWidget(float priceIncrement, QWidget* parent) :
    IOrderWavesWidget(parent),
    mBids(),
    mSkips(),
    mAsks(),
    mMaxQuantity(-1),
    mMinPrice(-1),
    mMaxPrice(-1),
    mPriceIncrement(priceIncrement)
{
    qDebug() << "Create OrderWavesWidget";

    setScene(&mScene);
}

OrderWavesWidget::~OrderWavesWidget()
{
    qDebug() << "Destroy OrderWavesWidget";
}

void OrderWavesWidget::orderBookChanged(const OrderBook& orderBook)
{
    for (int i = 0; i < orderBook.bids.size(); ++i)
    {
        qint32 quantity = orderBook.bids.at(i).quantity;

        if (quantity > mMaxQuantity)
        {
            mMaxQuantity = quantity;
        }
    }

    for (int i = 0; i < orderBook.asks.size(); ++i)
    {
        qint32 quantity = orderBook.asks.at(i).quantity;

        if (quantity > mMaxQuantity)
        {
            mMaxQuantity = quantity;
        }
    }

    float minPrice = !orderBook.bids.isEmpty() ? orderBook.bids.last().price : orderBook.price;
    float maxPrice = !orderBook.asks.isEmpty() ? orderBook.asks.last().price : orderBook.price;

    if (mMinPrice < 0 || minPrice < mMinPrice)
    {
        mMinPrice = minPrice;
    }

    if (mMaxPrice < 0 || maxPrice < mMaxPrice)
    {
        mMaxPrice = maxPrice;
    }

    qInfo() << mMaxQuantity << mMinPrice << mMaxPrice << mPriceIncrement;
}

void OrderWavesWidget::reset()
{
}
