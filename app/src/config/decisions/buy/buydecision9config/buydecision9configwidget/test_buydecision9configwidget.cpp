#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/buydecision9configwidget.h"
#include "ui_buydecision9configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision9config/ibuydecision9config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
class Test_BuyDecision9ConfigWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision9ConfigMock = new StrictMock<BuyDecision9ConfigMock>();

        configWidget = new BuyDecision9ConfigWidget(buyDecision9ConfigMock);
    }

    void TearDown() override
    {
        delete configWidget;
        delete buyDecision9ConfigMock;
    }

    BuyDecision9ConfigWidget*           configWidget;
    StrictMock<BuyDecision9ConfigMock>* buyDecision9ConfigMock;
};



TEST_F(Test_BuyDecision9ConfigWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->rsiMonthDoubleSpinBox->blockSignals(true);
    configWidget->ui->rsiWeekDoubleSpinBox->blockSignals(true);
    configWidget->ui->rsiDayDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(*buyDecision9ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsiMonth()).WillOnce(Return(2.1f));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsiWeek()).WillOnce(Return(3.1f));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsiDay()).WillOnce(Return(4.1f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),     true);
    ASSERT_NEAR(configWidget->ui->rsiMonthDoubleSpinBox->value(), 2.1f, 0.0001f);
    ASSERT_NEAR(configWidget->ui->rsiWeekDoubleSpinBox->value(),  3.1f, 0.0001f);
    ASSERT_NEAR(configWidget->ui->rsiDayDoubleSpinBox->value(),   4.1f, 0.0001f);
    // clang-format on

    EXPECT_CALL(*buyDecision9ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsiMonth()).WillOnce(Return(5.3f));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsiWeek()).WillOnce(Return(6.3f));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsiDay()).WillOnce(Return(7.3f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),     false);
    ASSERT_NEAR(configWidget->ui->rsiMonthDoubleSpinBox->value(), 5.3f, 0.0001f);
    ASSERT_NEAR(configWidget->ui->rsiWeekDoubleSpinBox->value(),  6.3f, 0.0001f);
    ASSERT_NEAR(configWidget->ui->rsiDayDoubleSpinBox->value(),   7.3f, 0.0001f);
    // clang-format on
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_makeReadOnly)
{
    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget->ui->rsiMonthDoubleSpinBox->isReadOnly(),                              false);
    ASSERT_EQ(configWidget->ui->rsiWeekDoubleSpinBox->isReadOnly(),                               false);
    ASSERT_EQ(configWidget->ui->rsiDayDoubleSpinBox->isReadOnly(),                                false);
    // clang-format on

    configWidget->makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget->ui->rsiMonthDoubleSpinBox->isReadOnly(),                              true);
    ASSERT_EQ(configWidget->ui->rsiWeekDoubleSpinBox->isReadOnly(),                               true);
    ASSERT_EQ(configWidget->ui->rsiDayDoubleSpinBox->isReadOnly(),                                true);
    // clang-format on
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->enabledCheckBox->setChecked(false);
    configWidget->ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(*buyDecision9ConfigMock, setEnabled(true));
    configWidget->ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget->ui->rsiMonthDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget->ui->rsiWeekDoubleSpinBox->isEnabled(),  true);
    ASSERT_EQ(configWidget->ui->rsiDayDoubleSpinBox->isEnabled(),   true);
    // clang-format on

    EXPECT_CALL(*buyDecision9ConfigMock, setEnabled(false));
    configWidget->ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget->ui->rsiMonthDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget->ui->rsiWeekDoubleSpinBox->isEnabled(),  false);
    ASSERT_EQ(configWidget->ui->rsiDayDoubleSpinBox->isEnabled(),   false);
    // clang-format on
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_on_rsiMonthDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->rsiMonthDoubleSpinBox->blockSignals(true);
    configWidget->ui->rsiMonthDoubleSpinBox->setValue(1.0f);
    configWidget->ui->rsiMonthDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsiMonth(2.0f));
    configWidget->ui->rsiMonthDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsiMonth(3.0f));
    configWidget->ui->rsiMonthDoubleSpinBox->setValue(3.0f);
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_on_rsiWeekDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->rsiWeekDoubleSpinBox->blockSignals(true);
    configWidget->ui->rsiWeekDoubleSpinBox->setValue(1.0f);
    configWidget->ui->rsiWeekDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsiWeek(2.0f));
    configWidget->ui->rsiWeekDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsiWeek(3.0f));
    configWidget->ui->rsiWeekDoubleSpinBox->setValue(3.0f);
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_on_rsiDayDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->rsiDayDoubleSpinBox->blockSignals(true);
    configWidget->ui->rsiDayDoubleSpinBox->setValue(1.0f);
    configWidget->ui->rsiDayDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsiDay(2.0f));
    configWidget->ui->rsiDayDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsiDay(3.0f));
    configWidget->ui->rsiDayDoubleSpinBox->setValue(3.0f);
}
// NOLINTEND(readability-magic-numbers)
