#include "src/widgets/instrumentwidget/instrumentwidget.h"
#include "ui_instrumentwidget.h"

#include <QEnterEvent>
#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
class Test_InstrumentWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        widget = new InstrumentWidget(userStorageMock);
    }

    void TearDown() override
    {
        delete widget;
        delete userStorageMock;
    }

    InstrumentWidget*            widget;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_InstrumentWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_InstrumentWidget, Test_enterEvent_and_leaveEvent)
{
    const QPointF pos;

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

TEST_F(Test_InstrumentWidget, Test_setIcon)
{
    widget->setIcon(QIcon());
}

TEST_F(Test_InstrumentWidget, Test_setQualInvestor_and_forQualInvestorFlag)
{
    const InSequence seq;

    widget->show();

    // clang-format off
    ASSERT_EQ(widget->forQualInvestorFlag(),      false);
    ASSERT_EQ(widget->ui->lockLabel->isVisible(), false);
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

TEST_F(Test_InstrumentWidget, Test_setText_and_text)
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

TEST_F(Test_InstrumentWidget, Test_setFullText_and_fullText)
{
    ASSERT_EQ(widget->fullText(), "");

    widget->setFullText("AAAAA");
    ASSERT_EQ(widget->fullText(), "AAAAA");

    widget->setFullText("BBBBB");
    ASSERT_EQ(widget->fullText(), "BBBBB");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
