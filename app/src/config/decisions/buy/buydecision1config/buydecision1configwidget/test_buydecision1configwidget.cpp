#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.h"
#include "ui_buydecision1configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_BuyDecision1ConfigWidget, Test_constructor_and_destructor)
{
    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    BuyDecision1ConfigWidget configWidget(&buyDecision1ConfigMock);
}

TEST(Test_BuyDecision1ConfigWidget, Test_updateUiFromConfig)
{
    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    BuyDecision1ConfigWidget configWidget(&buyDecision1ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->priceFallDoubleSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(buyDecision1ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(buyDecision1ConfigMock, getPriceFall()).WillOnce(Return(2.1f));
    EXPECT_CALL(buyDecision1ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      true);
    ASSERT_NEAR(configWidget.ui->priceFallDoubleSpinBox->value(), 2.1f, 0.01f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          3);

    EXPECT_CALL(buyDecision1ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(buyDecision1ConfigMock, getPriceFall()).WillOnce(Return(5.3f));
    EXPECT_CALL(buyDecision1ConfigMock, getDuration()).WillOnce(Return(2));

    configWidget.updateUiFromConfig();

    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      false);
    ASSERT_NEAR(configWidget.ui->priceFallDoubleSpinBox->value(), 5.3f, 0.01f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          2);
}

TEST(Test_BuyDecision1ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    BuyDecision1ConfigWidget configWidget(&buyDecision1ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(buyDecision1ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);
    ASSERT_EQ(configWidget.ui->priceFallDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(), true);

    EXPECT_CALL(buyDecision1ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);
    ASSERT_EQ(configWidget.ui->priceFallDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(), false);
}

TEST(Test_BuyDecision1ConfigWidget, Test_on_priceFallDoubleSpinBox_valueChanged)
{
    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    BuyDecision1ConfigWidget configWidget(&buyDecision1ConfigMock);

    configWidget.ui->priceFallDoubleSpinBox->blockSignals(true);
    configWidget.ui->priceFallDoubleSpinBox->setValue(1.0f);
    configWidget.ui->priceFallDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision1ConfigMock, setPriceFall(2.0f));
    configWidget.ui->priceFallDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(buyDecision1ConfigMock, setPriceFall(3.0f));
    configWidget.ui->priceFallDoubleSpinBox->setValue(3.0f);
}

TEST(Test_BuyDecision1ConfigWidget, Test_on_durationSpinBox_valueChanged)
{
    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    BuyDecision1ConfigWidget configWidget(&buyDecision1ConfigMock);

    configWidget.ui->durationSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->setValue(1);
    configWidget.ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision1ConfigMock, setDuration(2));
    configWidget.ui->durationSpinBox->setValue(2);

    EXPECT_CALL(buyDecision1ConfigMock, setDuration(3));
    configWidget.ui->durationSpinBox->setValue(3);
}
