#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/buydecision4configwidget.h"
#include "ui_buydecision4configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision4config/ibuydecision4config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_BuyDecision4ConfigWidget, Test_constructor_and_destructor)
{
    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    const BuyDecision4ConfigWidget configWidget(&buyDecision4ConfigMock);
}

TEST(Test_BuyDecision4ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    const BuyDecision4ConfigWidget configWidget(&buyDecision4ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->priceFallDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseYieldDoubleSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(buyDecision4ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(buyDecision4ConfigMock, getPriceFall()).WillOnce(Return(2.1f));
    EXPECT_CALL(buyDecision4ConfigMock, getLoseYield()).WillOnce(Return(3.1f));
    EXPECT_CALL(buyDecision4ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      true);
    ASSERT_NEAR(configWidget.ui->priceFallDoubleSpinBox->value(), 2.1f, 0.0001f);
    ASSERT_NEAR(configWidget.ui->loseYieldDoubleSpinBox->value(), 3.1f, 0.0001f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          3);
    // clang-format on

    EXPECT_CALL(buyDecision4ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(buyDecision4ConfigMock, getPriceFall()).WillOnce(Return(5.3f));
    EXPECT_CALL(buyDecision4ConfigMock, getLoseYield()).WillOnce(Return(6.3f));
    EXPECT_CALL(buyDecision4ConfigMock, getDuration()).WillOnce(Return(2));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),      false);
    ASSERT_NEAR(configWidget.ui->priceFallDoubleSpinBox->value(), 5.3f, 0.0001f);
    ASSERT_NEAR(configWidget.ui->loseYieldDoubleSpinBox->value(), 6.3f, 0.0001f);
    ASSERT_EQ(configWidget.ui->durationSpinBox->value(),          2);
    // clang-format on
}

TEST(Test_BuyDecision4ConfigWidget, Test_makeReadOnly)
{
    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    const BuyDecision4ConfigWidget configWidget(&buyDecision4ConfigMock);

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget.ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget.ui->priceFallDoubleSpinBox->isReadOnly(),                             false);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isReadOnly(),                             false);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isReadOnly(),                                    false);
    // clang-format on

    configWidget.makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget.ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget.ui->priceFallDoubleSpinBox->isReadOnly(),                             true);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isReadOnly(),                             true);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isReadOnly(),                                    true);
    // clang-format on
}

TEST(Test_BuyDecision4ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    const BuyDecision4ConfigWidget configWidget(&buyDecision4ConfigMock);

    configWidget.ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->enabledCheckBox->setChecked(false);
    configWidget.ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(buyDecision4ConfigMock, setEnabled(true));
    configWidget.ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget.ui->priceFallDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),        true);
    // clang-format on

    EXPECT_CALL(buyDecision4ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget.ui->priceFallDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->durationSpinBox->isEnabled(),        false);
    // clang-format on
}

TEST(Test_BuyDecision4ConfigWidget, Test_on_priceFallDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    const BuyDecision4ConfigWidget configWidget(&buyDecision4ConfigMock);

    configWidget.ui->priceFallDoubleSpinBox->blockSignals(true);
    configWidget.ui->priceFallDoubleSpinBox->setValue(1.0f);
    configWidget.ui->priceFallDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision4ConfigMock, setPriceFall(2.0f));
    configWidget.ui->priceFallDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(buyDecision4ConfigMock, setPriceFall(3.0f));
    configWidget.ui->priceFallDoubleSpinBox->setValue(3.0f);
}

TEST(Test_BuyDecision4ConfigWidget, Test_on_loseYieldDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    const BuyDecision4ConfigWidget configWidget(&buyDecision4ConfigMock);

    configWidget.ui->loseYieldDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseYieldDoubleSpinBox->setValue(1.0f);
    configWidget.ui->loseYieldDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision4ConfigMock, setLoseYield(2.0f));
    configWidget.ui->loseYieldDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(buyDecision4ConfigMock, setLoseYield(3.0f));
    configWidget.ui->loseYieldDoubleSpinBox->setValue(3.0f);
}

TEST(Test_BuyDecision4ConfigWidget, Test_on_durationSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<BuyDecision4ConfigMock> buyDecision1ConfigMock;

    const BuyDecision4ConfigWidget configWidget(&buyDecision1ConfigMock);

    configWidget.ui->durationSpinBox->blockSignals(true);
    configWidget.ui->durationSpinBox->setValue(1);
    configWidget.ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(buyDecision1ConfigMock, setDuration(2));
    configWidget.ui->durationSpinBox->setValue(2);

    EXPECT_CALL(buyDecision1ConfigMock, setDuration(3));
    configWidget.ui->durationSpinBox->setValue(3);
}
// NOLINTEND(readability-magic-numbers)
