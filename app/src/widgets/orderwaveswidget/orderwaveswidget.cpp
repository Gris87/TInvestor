#include "src/widgets/orderwaveswidget/orderwaveswidget.h"

#include <QDebug>

#define SCENE_HEIGHT       580
#define BAR_WIDTH          80
#define AXIS_LINE_HEIGHT   10
#define AXIS_MARKER_OFFSET 10
#define LINE_COLOR         "#FFFFFF"
#define AXIS_LINE_COLOR    "#FFFFFF"
#define AXIS_MARKER_COLOR  "#FFFFFF"



OrderWavesWidget::OrderWavesWidget(float priceIncrement, QWidget* parent) :
    IOrderWavesWidget(parent),
    mScene(),
    mBottomLine(),
    mAxisLines(),
    mAxisMarkers(),
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

    int amountOfBars = qRound((mMaxPrice - mMinPrice) / mPriceIncrement) + 1;

    if (mBottomLine != nullptr)
    {
        mBottomLine->setLine(0, 0, amountOfBars * BAR_WIDTH, 0);
    }
    else
    {
        mBottomLine = mScene.addLine(0, 0, amountOfBars * BAR_WIDTH, 0, QPen(QColor(LINE_COLOR)));
    }

    for (int i = 0; i < amountOfBars; ++i)
    {
        int axisX = i * BAR_WIDTH + BAR_WIDTH / 2;

        QGraphicsSimpleTextItem* axisMarker;

        if (i < mAxisLines.size())
        {
            axisMarker = mAxisMarkers.at(i);

            mAxisLines.at(i)->setLine(axisX, -AXIS_LINE_HEIGHT / 2, axisX, AXIS_LINE_HEIGHT / 2);
            axisMarker->setText("50000000.00");
        }
        else
        {
            axisMarker = mScene.addSimpleText("50000000.00");
            axisMarker->setBrush(QBrush(QColor(AXIS_MARKER_COLOR)));

            mAxisLines.append(
                mScene.addLine(axisX, -AXIS_LINE_HEIGHT / 2, axisX, AXIS_LINE_HEIGHT / 2, QPen(QColor(AXIS_LINE_COLOR)))
            );
            mAxisMarkers.append(axisMarker);
        }

        axisMarker->setPos(axisX - axisMarker->boundingRect().width() / 2, AXIS_MARKER_OFFSET);
    }
}

void OrderWavesWidget::reset()
{
}
