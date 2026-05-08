#include "src/widgets/notificationwidget/notificationwidget.h"
#include "ui_notificationwidget.h"

#include <QMouseEvent>
#include <gtest/gtest.h>



class Test_NotificationWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        notificationWidget = new NotificationWidget("...");
    }

    void TearDown() override
    {
        delete notificationWidget;
    }

    NotificationWidget* notificationWidget;
};



TEST_F(Test_NotificationWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationWidget, Test_mouseReleaseEvent)
{
    const QPointF pos;

    QMouseEvent mouseEvent(QEvent::MouseMove, pos, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

    notificationWidget->mouseReleaseEvent(&mouseEvent);
}

TEST_F(Test_NotificationWidget, Test_deathTimerTicked)
{
    notificationWidget->deathTimerTicked();
}
