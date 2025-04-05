#include "src/widgets/tableitems/stock/stocktableitemwidget.h"
#include "ui_stocktableitemwidget.h"

#include <QEnterEvent>
#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_StockTableItemWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        widget = new StockTableItemWidget(userStorageMock);
    }

    void TearDown()
    {
        delete widget;
        delete userStorageMock;
    }

    StockTableItemWidget*        widget;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_StockTableItemWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_StockTableItemWidget, Test_enterEvent_and_leaveEvent)
{
    QPointF pos;

    QEnterEvent enterEvent(pos, pos, pos);
    QEvent      leaveEvent(QEvent::Leave);

    widget->setFullText("HOORAY");
    widget->show();

    // clang-format off
    ASSERT_EQ(widget->hoverTextWidget,            nullptr);
    ASSERT_EQ(widget->ui->nameLabel->isVisible(), true);
    // clang-format on

    widget->enterEvent(&enterEvent);

    // clang-format off
    ASSERT_NE(widget->hoverTextWidget,              nullptr);
    ASSERT_EQ(widget->hoverTextWidget->text(),      "HOORAY");
    ASSERT_EQ(widget->hoverTextWidget->isVisible(), true);
    ASSERT_EQ(widget->ui->nameLabel->isVisible(),   false);
    // clang-format on

    widget->leaveEvent(&leaveEvent);

    // clang-format off
    ASSERT_EQ(widget->hoverTextWidget,            nullptr);
    ASSERT_EQ(widget->ui->nameLabel->isVisible(), true);
    // clang-format on

    widget->enterEvent(&enterEvent);

    // clang-format off
    ASSERT_NE(widget->hoverTextWidget,              nullptr);
    ASSERT_EQ(widget->hoverTextWidget->text(),      "HOORAY");
    ASSERT_EQ(widget->hoverTextWidget->isVisible(), true);
    ASSERT_EQ(widget->ui->nameLabel->isVisible(),   false);
    // clang-format on
}

TEST_F(Test_StockTableItemWidget, Test_setIcon)
{
    widget->setIcon(QIcon());
}

TEST_F(Test_StockTableItemWidget, Test_setQualInvestor_and_forQualInvestorFlag)
{
    InSequence seq;

    widget->show();

    // clang-format off
    ASSERT_EQ(widget->forQualInvestorFlag(),      false);
    ASSERT_EQ(widget->ui->lockLabel->isVisible(), true);
    // clang-format on

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(false));

    widget->setQualInvestor(true);

    // clang-format off
    ASSERT_EQ(widget->forQualInvestorFlag(),      true);
    ASSERT_EQ(widget->ui->lockLabel->isVisible(), true);
    // clang-format on

    EXPECT_CALL(*userStorageMock, isQualified()).WillOnce(Return(true));

    widget->setQualInvestor(true);

    // clang-format off
    ASSERT_EQ(widget->forQualInvestorFlag(),      true);
    ASSERT_EQ(widget->ui->lockLabel->isVisible(), false);
    // clang-format on

    widget->setQualInvestor(false);

    // clang-format off
    ASSERT_EQ(widget->forQualInvestorFlag(),      false);
    ASSERT_EQ(widget->ui->lockLabel->isVisible(), false);
    // clang-format on
}

TEST_F(Test_StockTableItemWidget, Test_setText_and_text)
{
    // clang-format off
    ASSERT_EQ(widget->text(),                "Name");
    ASSERT_EQ(widget->ui->nameLabel->text(), "Name");
    // clang-format on

    widget->setText("AAAAA");

    // clang-format off
    ASSERT_EQ(widget->text(),                "AAAAA");
    ASSERT_EQ(widget->ui->nameLabel->text(), "AAAAA");
    // clang-format on

    widget->setText("BBBBB");

    // clang-format off
    ASSERT_EQ(widget->text(),                "BBBBB");
    ASSERT_EQ(widget->ui->nameLabel->text(), "BBBBB");
    // clang-format on
}

TEST_F(Test_StockTableItemWidget, Test_setFullText_and_fullText)
{
    ASSERT_EQ(widget->fullText(), "");

    widget->setFullText("AAAAA");
    ASSERT_EQ(widget->fullText(), "AAAAA");

    widget->setFullText("BBBBB");
    ASSERT_EQ(widget->fullText(), "BBBBB");
}

TEST_F(Test_StockTableItemWidget, Test_less)
{
    StockTableItemWidget widget2(userStorageMock);

    widget->setText("AAAAA");
    widget2.setText("BBBBB");

    ASSERT_LT(*widget, widget2);

    widget->setText("CCCCC");

    ASSERT_LT(widget2, *widget);
}
