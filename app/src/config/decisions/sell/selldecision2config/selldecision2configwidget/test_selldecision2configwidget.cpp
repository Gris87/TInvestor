#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.h"
#include "ui_selldecision2configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_SellDecision2ConfigWidget, Test_constructor_and_destructor)
{
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);
}

TEST(Test_SellDecision2ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->incomeAboveDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision2ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision2ConfigMock, getIncomeAbove()).WillOnce(Return(2.1f));
    EXPECT_CALL(sellDecision2ConfigMock, getLoseIncome()).WillOnce(Return(3.1f));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),        true);
    ASSERT_NEAR(configWidget.ui->incomeAboveDoubleSpinBox->value(), 2.1f, 0.0001f);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(),  3.1f, 0.0001f);
    // clang-format on

    EXPECT_CALL(sellDecision2ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(sellDecision2ConfigMock, getIncomeAbove()).WillOnce(Return(5.3f));
    EXPECT_CALL(sellDecision2ConfigMock, getLoseIncome()).WillOnce(Return(6.3f));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),        false);
    ASSERT_NEAR(configWidget.ui->incomeAboveDoubleSpinBox->value(), 5.3f, 0.0001f);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(),  6.3f, 0.0001f);
    // clang-format on
}

TEST(Test_SellDecision2ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(sellDecision2ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget.ui->incomeAboveDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(),  true);
    // clang-format on

    EXPECT_CALL(sellDecision2ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget.ui->incomeAboveDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(),  false);
    // clang-format on
}

TEST(Test_SellDecision2ConfigWidget, Test_on_incomeAboveDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);

    configWidget.ui->incomeAboveDoubleSpinBox->blockSignals(true);
    configWidget.ui->incomeAboveDoubleSpinBox->setValue(1.0f);
    configWidget.ui->incomeAboveDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision2ConfigMock, setIncomeAbove(2.0f));
    configWidget.ui->incomeAboveDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(sellDecision2ConfigMock, setIncomeAbove(3.0f));
    configWidget.ui->incomeAboveDoubleSpinBox->setValue(3.0f);
}

TEST(Test_SellDecision2ConfigWidget, Test_on_loseIncomeDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);

    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(1.0f);
    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision2ConfigMock, setLoseIncome(2.0f));
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(sellDecision2ConfigMock, setLoseIncome(3.0f));
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(3.0f);
}
// NOLINTEND(readability-magic-numbers)
