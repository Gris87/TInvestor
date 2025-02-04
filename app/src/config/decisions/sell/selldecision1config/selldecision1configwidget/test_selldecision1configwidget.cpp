#include "src/config/decisions/sell/selldecision1configwidget.h"
#include "ui_selldecision1configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/iselldecision1config_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_SellDecision1ConfigWidget, Test_constructor_and_destructor)
{
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;

    SellDecision1ConfigWidget configWidget(&sellDecision1ConfigMock);
}

TEST(Test_SellDecision1ConfigWidget, Test_updateUiFromConfig)
{
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;

    SellDecision1ConfigWidget configWidget(&sellDecision1ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->incomeAboveDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision1ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision1ConfigMock, getIncomeAbove()).WillOnce(Return(2.1f));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),        true);
    ASSERT_NEAR(configWidget.ui->incomeAboveDoubleSpinBox->value(), 2.1f, 0.01f);

    EXPECT_CALL(sellDecision1ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(sellDecision1ConfigMock, getIncomeAbove()).WillOnce(Return(5.3f));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),        false);
    ASSERT_NEAR(configWidget.ui->incomeAboveDoubleSpinBox->value(), 5.3f, 0.01f);
}

TEST(Test_SellDecision1ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;

    SellDecision1ConfigWidget configWidget(&sellDecision1ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(sellDecision1ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);
    ASSERT_EQ(configWidget.ui->incomeAboveDoubleSpinBox->isEnabled(), true);

    EXPECT_CALL(sellDecision1ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);
    ASSERT_EQ(configWidget.ui->incomeAboveDoubleSpinBox->isEnabled(), false);
}

TEST(Test_SellDecision1ConfigWidget, Test_on_incomeAboveDoubleSpinBox_valueChanged)
{
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;

    SellDecision1ConfigWidget configWidget(&sellDecision1ConfigMock);

    configWidget.ui->incomeAboveDoubleSpinBox->blockSignals(true);
    configWidget.ui->incomeAboveDoubleSpinBox->setValue(1.0f);
    configWidget.ui->incomeAboveDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision1ConfigMock, setIncomeAbove(2.0f));
    configWidget.ui->incomeAboveDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(sellDecision1ConfigMock, setIncomeAbove(3.0f));
    configWidget.ui->incomeAboveDoubleSpinBox->setValue(3.0f);
}
