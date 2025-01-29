#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision3configwidget.h"
#include "src/config/decisions/sell/iselldecision3config_mock.h"
#include "ui_selldecision3configwidget.h"



using ::testing::StrictMock;
using ::testing::Return;



TEST(Test_SellDecision3ConfigWidget, Test_constructor_and_destructor)
{
    SellDecision3ConfigWidget configWidget;
}

TEST(Test_SellDecision3ConfigWidget, Test_setSellDecision3Config)
{
    SellDecision3ConfigWidget configWidget;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;
    configWidget.setSellDecision3Config(&sellDecision3ConfigMock);
}

TEST(Test_SellDecision3ConfigWidget, Test_updateUiFromConfig)
{
    SellDecision3ConfigWidget configWidget;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;
    configWidget.setSellDecision3Config(&sellDecision3ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->loseIncomeDoubleSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision3ConfigMock, getLoseIncome()).WillOnce(Return(2.1f));
    EXPECT_CALL(sellDecision3ConfigMock, getDuration()).WillOnce(Return(4));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),       true);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(), 2.1f, 0.01f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),           4);

    EXPECT_CALL(sellDecision3ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(sellDecision3ConfigMock, getLoseIncome()).WillOnce(Return(5.3f));
    EXPECT_CALL(sellDecision3ConfigMock, getDuration()).WillOnce(Return(7));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),       false);
    ASSERT_NEAR(configWidget.ui->loseIncomeDoubleSpinBox->value(), 5.3f, 0.01f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),           7);
}

TEST(Test_SellDecision3ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    SellDecision3ConfigWidget configWidget;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;
    configWidget.setSellDecision3Config(&sellDecision3ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(sellDecision3ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(), true);

    EXPECT_CALL(sellDecision3ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);
    ASSERT_EQ(configWidget.ui->loseIncomeDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(), false);
}

TEST(Test_SellDecision3ConfigWidget, Test_on_loseIncomeDoubleSpinBox_valueChanged)
{
    SellDecision3ConfigWidget configWidget;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;
    configWidget.setSellDecision3Config(&sellDecision3ConfigMock);

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
    SellDecision3ConfigWidget configWidget;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;
    configWidget.setSellDecision3Config(&sellDecision3ConfigMock);

    configWidget.ui->durationSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->setValue(1);
    configWidget.ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision3ConfigMock, setDuration(2));
    configWidget.ui->durationSpinBox->setValue(2);

    EXPECT_CALL(sellDecision3ConfigMock, setDuration(3));
    configWidget.ui->durationSpinBox->setValue(3);
}
