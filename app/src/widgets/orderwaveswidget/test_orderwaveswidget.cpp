#include "src/widgets/orderwaveswidget/orderwaveswidget.h"

#include <QScrollBar>
#include <gtest/gtest.h>



class Test_OrderWavesWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        orderWavesWidget = new OrderWavesWidget(2, 0.01f);
    }

    void TearDown() override
    {
        delete orderWavesWidget;
    }

    OrderWavesWidget* orderWavesWidget;
};



TEST_F(Test_OrderWavesWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_OrderWavesWidget, Test_zoom)
{
    QTransform transform = orderWavesWidget->transform();

    ASSERT_NEAR(transform.m11(), 1.0, 0.0001f);
    ASSERT_NEAR(transform.m12(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m13(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m21(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m22(), 1.0, 0.0001f);
    ASSERT_NEAR(transform.m23(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m31(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m32(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m33(), 1.0, 0.0001f);

    orderWavesWidget->zoom(2);

    transform = orderWavesWidget->transform();

    ASSERT_NEAR(transform.m11(), 2.0, 0.0001f);
    ASSERT_NEAR(transform.m12(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m13(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m21(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m22(), 2.0, 0.0001f);
    ASSERT_NEAR(transform.m23(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m31(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m32(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m33(), 1.0, 0.0001f);
}

TEST_F(Test_OrderWavesWidget, Test_eventFilter)
{
    QPointF pos;

    QMouseEvent mouseEvent(QEvent::MouseMove, pos, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QWheelEvent wheelEvent(pos, pos, QPoint(0, 0), QPoint(0, 0), Qt::LeftButton, Qt::NoModifier, Qt::NoScrollPhase, false);

    ASSERT_EQ(orderWavesWidget->eventFilter(orderWavesWidget->viewport(), &mouseEvent), false);
    ASSERT_EQ(orderWavesWidget->eventFilter(orderWavesWidget->viewport(), &wheelEvent), true);
}

TEST_F(Test_OrderWavesWidget, Test_orderBookChanged)
{
    OrderBook orderBook1;

    orderBook1.timestamp = 100;
    orderBook1.price     = 1000.0f;

    orderWavesWidget->orderBookChanged(orderBook1);

    OrderBook orderBook2;

    orderBook2.timestamp = 100;
    orderBook2.price     = 1000.0f;

    for (int i = 0; i < 50; ++i)
    {
        OrderBookData bid2;

        bid2.quantity = 5 + i;
        bid2.price    = 998.25f - 0.01f * i;

        orderBook2.bids.append(bid2);
    }

    for (int i = 0; i < 50; ++i)
    {
        OrderBookData ask2;

        ask2.quantity = 15 + i;
        ask2.price    = 1001.05f + 0.01f * i;

        orderBook2.asks.append(ask2);
    }

    orderWavesWidget->orderBookChanged(orderBook2);

    OrderBook orderBook3;

    orderBook3.timestamp = 100;
    orderBook3.price     = 1000.0f;

    for (int i = 0; i < 50; ++i)
    {
        OrderBookData bid3;

        bid3.quantity = 25 + i;
        bid3.price    = 999.95f - 0.01f * i;

        orderBook3.bids.append(bid3);
    }

    for (int i = 0; i < 50; ++i)
    {
        OrderBookData ask3;

        ask3.quantity = 15 + i;
        ask3.price    = 1000.25f + 0.01f * i;

        orderBook3.asks.append(ask3);
    }

    orderWavesWidget->orderBookChanged(orderBook3);

    OrderBook orderBook4;

    orderBook4.timestamp = 100;
    orderBook4.price     = 1000.0f;

    for (int i = 0; i < 50; ++i)
    {
        OrderBookData bid4;

        bid4.quantity = 25 + i;
        bid4.price    = 999.95f - 0.01f * i;

        orderBook4.bids.append(bid4);
    }

    for (int i = 0; i < 50; ++i)
    {
        OrderBookData ask4;

        ask4.quantity = 35 + i;
        ask4.price    = 1000.05f + 0.01f * i;

        orderBook4.asks.append(ask4);
    }

    orderWavesWidget->orderBookChanged(orderBook4);

    OrderBook orderBook5;

    OrderBookData bid5;

    bid5.quantity = 5;
    bid5.price    = 0.05f;

    orderBook5.timestamp = 100;
    orderBook5.price     = 1000.0f;
    orderBook5.bids.append(bid5);

    orderWavesWidget->orderBookChanged(orderBook5);

    OrderBook orderBook6;

    OrderBookData ask6;

    ask6.quantity = 5;
    ask6.price    = 5000.0f;

    orderBook6.timestamp = 100;
    orderBook6.price     = 1000.0f;
    orderBook6.asks.append(ask6);

    orderWavesWidget->orderBookChanged(orderBook6);

    orderWavesWidget->show();
    orderWavesWidget->horizontalScrollBar()->setValue(50);
}

TEST_F(Test_OrderWavesWidget, Test_reset)
{
    QTransform transform = orderWavesWidget->transform();

    ASSERT_NEAR(transform.m11(), 1.0, 0.0001f);
    ASSERT_NEAR(transform.m12(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m13(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m21(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m22(), 1.0, 0.0001f);
    ASSERT_NEAR(transform.m23(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m31(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m32(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m33(), 1.0, 0.0001f);

    orderWavesWidget->zoom(2);

    transform = orderWavesWidget->transform();

    ASSERT_NEAR(transform.m11(), 2.0, 0.0001f);
    ASSERT_NEAR(transform.m12(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m13(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m21(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m22(), 2.0, 0.0001f);
    ASSERT_NEAR(transform.m23(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m31(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m32(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m33(), 1.0, 0.0001f);

    orderWavesWidget->reset();

    transform = orderWavesWidget->transform();

    ASSERT_NEAR(transform.m11(), 1.0, 0.0001f);
    ASSERT_NEAR(transform.m12(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m13(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m21(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m22(), 1.0, 0.0001f);
    ASSERT_NEAR(transform.m23(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m31(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m32(), 0.0, 0.0001f);
    ASSERT_NEAR(transform.m33(), 1.0, 0.0001f);
}

TEST_F(Test_OrderWavesWidget, Test_setupArrow)
{
    orderWavesWidget->setupArrow(-1.0f, -1.0f, 100.0f);
}

TEST_F(Test_OrderWavesWidget, Test_calculateCurrentPrice)
{
    ASSERT_NEAR(orderWavesWidget->calculateCurrentPrice(-1.0f, -1.0f), 0.0f, 0.0001f);
}
