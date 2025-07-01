#include "src/widgets/accountchartwidget/charttooltip.h"

#include <QPainter>
#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_ChartTooltip : public ::testing::Test
{
protected:
    void SetUp() override
    {
        chart = new QChart();

        tooltip = new ChartTooltip(chart);
    }

    void TearDown() override
    {
        delete tooltip;
        delete chart;
    }

    ChartTooltip* tooltip;
    QChart*       chart;
};



TEST_F(Test_ChartTooltip, Test_constructor_and_destructor)
{
}

TEST_F(Test_ChartTooltip, Test_setText_and_text)
{
    tooltip->setText("Hello");

    ASSERT_EQ(tooltip->text(), "Hello");

    tooltip->setText("Babushka");

    ASSERT_EQ(tooltip->text(), "Babushka");
}

TEST_F(Test_ChartTooltip, Test_setAnchor_and_updateGeometry)
{
    tooltip->setAnchor(QPointF(0, 0));
    tooltip->updateGeometry();
}

TEST_F(Test_ChartTooltip, Test_boundingRect)
{
    tooltip->setText("Hello");

    QRectF rect = tooltip->boundingRect();

    qInfo() << rect; // TODO: Remove

#ifdef Q_OS_WINDOWS
    ASSERT_EQ(rect, QRectF(0, 0, 38, 26));
#else
    ASSERT_EQ(rect, QRectF(0, 0, 46, 28));
#endif
}

TEST_F(Test_ChartTooltip, Test_paint)
{
    tooltip->setText("Hello");

    QPainter painter;

    tooltip->paint(&painter, nullptr, nullptr);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
