#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.h"
#include "ui_selldecision3configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_SellDecision3ConfigWidget, Test_constructor_and_destructor)
{
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    const SellDecision3ConfigWidget configWidget(&sellDecision3ConfigMock);
}

TEST(Test_SellDecision3ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    const SellDecision3ConfigWidget configWidget(&sellDecision3ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision3ConfigMock, getDuration()).WillOnce(Return(4));
    EXPECT_CALL(sellDecision3ConfigMock, getLoseIncome()).WillOnce(Return(2.1f));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),       true);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(), 2.1f, 0.0001f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),           4);
    // clang-format on

    EXPECT_CALL(sellDecision3ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(sellDecision3ConfigMock, getDuration()).WillOnce(Return(7));
    EXPECT_CALL(sellDecision3ConfigMock, getLoseIncome()).WillOnce(Return(5.3f));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),       false);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(), 5.3f, 0.0001f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),           7);
    // clang-format on
}

TEST(Test_SellDecision3ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    const SellDecision3ConfigWidget configWidget(&sellDecision3ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(sellDecision3ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),         true);
    // clang-format on

    EXPECT_CALL(sellDecision3ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),         false);
    // clang-format on
}

TEST(Test_SellDecision3ConfigWidget, Test_on_loseIncomeDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    const SellDecision3ConfigWidget configWidget(&sellDecision3ConfigMock);

    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(1.0f);
    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision3ConfigMock, setLoseIncome(2.0f));
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(sellDecision3ConfigMock, setLoseIncome(3.0f));
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(3.0f);
}

TEST(Test_SellDecision3ConfigWidget, Test_on_durationSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    const SellDecision3ConfigWidget configWidget(&sellDecision3ConfigMock);

    configWidget.ui->durationSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->setValue(1);
    configWidget.ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision3ConfigMock, setDuration(2));
    configWidget.ui->durationSpinBox->setValue(2);

    EXPECT_CALL(sellDecision3ConfigMock, setDuration(3));
    configWidget.ui->durationSpinBox->setValue(3);
}
// NOLINTEND(readability-magic-numbers)
