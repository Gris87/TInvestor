#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision2configwidget.h"
#include "src/config/decisions/sell/iselldecision2config_mock.h"
#include "ui_selldecision2configwidget.h"



using ::testing::StrictMock;
using ::testing::Return;



TEST(Test_SellDecision2ConfigWidget, Test_constructor_and_destructor)
{
    SellDecision2ConfigWidget configWidget;
}

TEST(Test_SellDecision2ConfigWidget, Test_setSellDecision2Config)
{
    SellDecision2ConfigWidget configWidget;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    configWidget.setSellDecision2Config(&sellDecision2ConfigMock);
}

TEST(Test_SellDecision2ConfigWidget, Test_updateUiFromConfig)
{
    SellDecision2ConfigWidget configWidget;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    configWidget.setSellDecision2Config(&sellDecision2ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->incomeAboveDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision2ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision2ConfigMock, getIncomeAbove()).WillOnce(Return(2.1f));
    EXPECT_CALL(sellDecision2ConfigMock, getLoseIncome()).WillOnce(Return(3.1f));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),        true);
    ASSERT_NEAR(configWidget.ui->incomeAboveDoubleSpinBox->value(), 2.1f, 0.01f);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(),  3.1f, 0.01f);

    EXPECT_CALL(sellDecision2ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(sellDecision2ConfigMock, getIncomeAbove()).WillOnce(Return(5.3f));
    EXPECT_CALL(sellDecision2ConfigMock, getLoseIncome()).WillOnce(Return(6.3f));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),        false);
    ASSERT_NEAR(configWidget.ui->incomeAboveDoubleSpinBox->value(), 5.3f, 0.01f);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(),  6.3f, 0.01f);
}

TEST(Test_SellDecision2ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    SellDecision2ConfigWidget configWidget;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    configWidget.setSellDecision2Config(&sellDecision2ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(sellDecision2ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);
    ASSERT_EQ(configWidget.ui->incomeAboveDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(),  true);

    EXPECT_CALL(sellDecision2ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);
    ASSERT_EQ(configWidget.ui->incomeAboveDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(),  false);
}

TEST(Test_SellDecision2ConfigWidget, Test_on_incomeAboveDoubleSpinBox_valueChanged)
{
    SellDecision2ConfigWidget configWidget;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    configWidget.setSellDecision2Config(&sellDecision2ConfigMock);

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
    SellDecision2ConfigWidget configWidget;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    configWidget.setSellDecision2Config(&sellDecision2ConfigMock);

    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(1.0f);
    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision2ConfigMock, setLoseIncome(2.0f));
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(sellDecision2ConfigMock, setLoseIncome(3.0f));
    configWidget.ui->loseIncomeDoubleSpinBox->setValue(3.0f);
}
