#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.h"
#include "ui_buydecision3configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_BuyDecision3ConfigWidget, Test_constructor_and_destructor)
{
    StrictMock<BuyDecision3ConfigMock> buyDecision3ConfigMock;

    BuyDecision3ConfigWidget configWidget(&buyDecision3ConfigMock);
}

TEST(Test_BuyDecision3ConfigWidget, Test_updateUiFromConfig)
{
    StrictMock<BuyDecision3ConfigMock> buyDecision3ConfigMock;

    BuyDecision3ConfigWidget configWidget(&buyDecision3ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->priceRiseDoubleSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(buyDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(buyDecision3ConfigMock, getPriceRise()).WillOnce(Return(2.1f));
    EXPECT_CALL(buyDecision3ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      true);
    ASSERT_NEAR(configWidget.ui->priceRiseDoubleSpinBox->value(), 2.1f, 0.0001f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          3);
    // clang-format on

    EXPECT_CALL(buyDecision3ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(buyDecision3ConfigMock, getPriceRise()).WillOnce(Return(5.3f));
    EXPECT_CALL(buyDecision3ConfigMock, getDuration()).WillOnce(Return(2));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      false);
    ASSERT_NEAR(configWidget.ui->priceRiseDoubleSpinBox->value(), 5.3f, 0.0001f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          2);
    // clang-format on
}

TEST(Test_BuyDecision3ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    StrictMock<BuyDecision3ConfigMock> buyDecision3ConfigMock;

    BuyDecision3ConfigWidget configWidget(&buyDecision3ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(buyDecision3ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget.ui->priceRiseDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),        true);
    // clang-format on

    EXPECT_CALL(buyDecision3ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget.ui->priceRiseDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),        false);
    // clang-format on
}

TEST(Test_BuyDecision3ConfigWidget, Test_on_priceRiseDoubleSpinBox_valueChanged)
{
    StrictMock<BuyDecision3ConfigMock> buyDecision3ConfigMock;

    BuyDecision3ConfigWidget configWidget(&buyDecision3ConfigMock);

    configWidget.ui->priceRiseDoubleSpinBox->blockSignals(true);
    configWidget.ui->priceRiseDoubleSpinBox->setValue(1.0f);
    configWidget.ui->priceRiseDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision3ConfigMock, setPriceRise(2.0f));
    configWidget.ui->priceRiseDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(buyDecision3ConfigMock, setPriceRise(3.0f));
    configWidget.ui->priceRiseDoubleSpinBox->setValue(3.0f);
}

TEST(Test_BuyDecision3ConfigWidget, Test_on_durationSpinBox_valueChanged)
{
    StrictMock<BuyDecision3ConfigMock> buyDecision3ConfigMock;

    BuyDecision3ConfigWidget configWidget(&buyDecision3ConfigMock);

    configWidget.ui->durationSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->setValue(1);
    configWidget.ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision3ConfigMock, setDuration(2));
    configWidget.ui->durationSpinBox->setValue(2);

    EXPECT_CALL(buyDecision3ConfigMock, setDuration(3));
    configWidget.ui->durationSpinBox->setValue(3);
}
