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
    configWidget.ui->yieldAboveDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseYieldDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision2ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision2ConfigMock, getYieldAbove()).WillOnce(Return(2.1f));
    EXPECT_CALL(sellDecision2ConfigMock, getLoseYield()).WillOnce(Return(3.1f));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),       true);
    ASSERT_NEAR(configWidget.ui->yieldAboveDoubleSpinBox->value(), 2.1f, 0.0001f);
    ASSERT_NEAR(configWidget.ui->loseYieldDoubleSpinBox->value(),  3.1f, 0.0001f);
    // clang-format on

    EXPECT_CALL(sellDecision2ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(sellDecision2ConfigMock, getYieldAbove()).WillOnce(Return(5.3f));
    EXPECT_CALL(sellDecision2ConfigMock, getLoseYield()).WillOnce(Return(6.3f));

    configWidget.updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->isChecked(),       false);
    ASSERT_NEAR(configWidget.ui->yieldAboveDoubleSpinBox->value(), 5.3f, 0.0001f);
    ASSERT_NEAR(configWidget.ui->loseYieldDoubleSpinBox->value(),  6.3f, 0.0001f);
    // clang-format on
}

TEST(Test_SellDecision2ConfigWidget, Test_makeReadOnly)
{
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget.ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget.ui->yieldAboveDoubleSpinBox->isReadOnly(),                            false);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isReadOnly(),                             false);
    // clang-format on

    configWidget.makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget.ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget.ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget.ui->yieldAboveDoubleSpinBox->isReadOnly(),                            true);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isReadOnly(),                             true);
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
    ASSERT_EQ(configWidget.ui->yieldAboveDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isEnabled(),  true);
    // clang-format on

    EXPECT_CALL(sellDecision2ConfigMock, setEnabled(false));
    configWidget.ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget.ui->yieldAboveDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(configWidget.ui->loseYieldDoubleSpinBox->isEnabled(),  false);
    // clang-format on
}

TEST(Test_SellDecision2ConfigWidget, Test_on_yieldAboveDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);

    configWidget.ui->yieldAboveDoubleSpinBox->blockSignals(true);
    configWidget.ui->yieldAboveDoubleSpinBox->setValue(1.0f);
    configWidget.ui->yieldAboveDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision2ConfigMock, setYieldAbove(2.0f));
    configWidget.ui->yieldAboveDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(sellDecision2ConfigMock, setYieldAbove(3.0f));
    configWidget.ui->yieldAboveDoubleSpinBox->setValue(3.0f);
}

TEST(Test_SellDecision2ConfigWidget, Test_on_loseYieldDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    const SellDecision2ConfigWidget configWidget(&sellDecision2ConfigMock);

    configWidget.ui->loseYieldDoubleSpinBox->blockSignals(true);
    configWidget.ui->loseYieldDoubleSpinBox->setValue(1.0f);
    configWidget.ui->loseYieldDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(sellDecision2ConfigMock, setLoseYield(2.0f));
    configWidget.ui->loseYieldDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(sellDecision2ConfigMock, setLoseYield(3.0f));
    configWidget.ui->loseYieldDoubleSpinBox->setValue(3.0f);
}
// NOLINTEND(readability-magic-numbers)
