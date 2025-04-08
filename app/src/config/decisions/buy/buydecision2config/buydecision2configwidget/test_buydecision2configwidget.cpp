#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.h"
#include "ui_buydecision2configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_BuyDecision2ConfigWidget, Test_constructor_and_destructor)
{
    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    const BuyDecision2ConfigWidget configWidget(&buyDecision2ConfigMock);
}

TEST(Test_BuyDecision2ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    const BuyDecision2ConfigWidget configWidget(&buyDecision2ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->priceDiffDoubleSpinBox->blockSignals(true);
    configWidget.ui->amountOfTimesSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(buyDecision2ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(buyDecision2ConfigMock, getPriceDiff()).WillOnce(Return(2.1f));
    EXPECT_CALL(buyDecision2ConfigMock, getAmountOfTimes()).WillOnce(Return(1));
    EXPECT_CALL(buyDecision2ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      true);
    ASSERT_NEAR(configWidget.ui->priceDiffDoubleSpinBox->value(), 2.1f, 0.01f);
    ASSERT_EQ(configWidget.ui->amountOfTimesSpinBox->value(),     1);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          3);
    // clang-format on

    EXPECT_CALL(buyDecision2ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(buyDecision2ConfigMock, getPriceDiff()).WillOnce(Return(5.3f));
    EXPECT_CALL(buyDecision2ConfigMock, getAmountOfTimes()).WillOnce(Return(3));
    EXPECT_CALL(buyDecision2ConfigMock, getDuration()).WillOnce(Return(2));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      false);
    ASSERT_NEAR(configWidget.ui->priceDiffDoubleSpinBox->value(), 5.3f, 0.01f);
    ASSERT_EQ(configWidget.ui->amountOfTimesSpinBox->value(),     3);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          2);
    // clang-format on
}

TEST(Test_BuyDecision2ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    const BuyDecision2ConfigWidget configWidget(&buyDecision2ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(buyDecision2ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget.ui->priceDiffDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->amountOfTimesSpinBox->isEnabled(),   true);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),        true);
    // clang-format on

    EXPECT_CALL(buyDecision2ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget.ui->priceDiffDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->amountOfTimesSpinBox->isEnabled(),   false);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),        false);
    // clang-format on
}

TEST(Test_BuyDecision2ConfigWidget, Test_on_priceDiffDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    const BuyDecision2ConfigWidget configWidget(&buyDecision2ConfigMock);

    configWidget.ui->priceDiffDoubleSpinBox->blockSignals(true);
    configWidget.ui->priceDiffDoubleSpinBox->setValue(1.0f);
    configWidget.ui->priceDiffDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision2ConfigMock, setPriceDiff(2.0f));
    configWidget.ui->priceDiffDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(buyDecision2ConfigMock, setPriceDiff(3.0f));
    configWidget.ui->priceDiffDoubleSpinBox->setValue(3.0f);
}

TEST(Test_BuyDecision2ConfigWidget, Test_on_amountOfTimesSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    const BuyDecision2ConfigWidget configWidget(&buyDecision2ConfigMock);

    configWidget.ui->amountOfTimesSpinBox->blockSignals(true);
    configWidget.ui->amountOfTimesSpinBox->setValue(1);
    configWidget.ui->amountOfTimesSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision2ConfigMock, setAmountOfTimes(2));
    configWidget.ui->amountOfTimesSpinBox->setValue(2);

    EXPECT_CALL(buyDecision2ConfigMock, setAmountOfTimes(3));
    configWidget.ui->amountOfTimesSpinBox->setValue(3);
}

TEST(Test_BuyDecision2ConfigWidget, Test_on_durationSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    const BuyDecision2ConfigWidget configWidget(&buyDecision2ConfigMock);

    configWidget.ui->durationSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->setValue(1);
    configWidget.ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision2ConfigMock, setDuration(2));
    configWidget.ui->durationSpinBox->setValue(2);

    EXPECT_CALL(buyDecision2ConfigMock, setDuration(3));
    configWidget.ui->durationSpinBox->setValue(3);
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
