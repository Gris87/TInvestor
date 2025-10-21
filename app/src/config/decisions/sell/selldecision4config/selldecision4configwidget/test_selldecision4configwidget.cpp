#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/selldecision4configwidget.h"
#include "ui_selldecision4configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
class Test_SellDecision4ConfigWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision4ConfigMock = new StrictMock<SellDecision4ConfigMock>();

        configWidget = new SellDecision4ConfigWidget(sellDecision4ConfigMock);
    }

    void TearDown() override
    {
        delete configWidget;
        delete sellDecision4ConfigMock;
    }

    SellDecision4ConfigWidget*           configWidget;
    StrictMock<SellDecision4ConfigMock>* sellDecision4ConfigMock;
};



TEST_F(Test_SellDecision4ConfigWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision4ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->durationSpinBox->blockSignals(true);
    configWidget->ui->yieldAboveDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(*sellDecision4ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision4ConfigMock, getDuration()).WillOnce(Return(4));
    EXPECT_CALL(*sellDecision4ConfigMock, getYieldAbove()).WillOnce(Return(2.1f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),       true);
    ASSERT_EQ(configWidget->ui->durationSpinBox->value(),           4);
    ASSERT_NEAR(configWidget->ui->yieldAboveDoubleSpinBox->value(), 2.1f, 0.0001f);
    // clang-format on

    EXPECT_CALL(*sellDecision4ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(*sellDecision4ConfigMock, getDuration()).WillOnce(Return(7));
    EXPECT_CALL(*sellDecision4ConfigMock, getYieldAbove()).WillOnce(Return(5.3f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),       false);
    ASSERT_EQ(configWidget->ui->durationSpinBox->value(),           7);
    ASSERT_NEAR(configWidget->ui->yieldAboveDoubleSpinBox->value(), 5.3f, 0.0001f);
    // clang-format on
}

TEST_F(Test_SellDecision4ConfigWidget, Test_makeReadOnly)
{
    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget->ui->durationSpinBox->isReadOnly(),                                    false);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isReadOnly(),                            false);
    // clang-format on

    configWidget->makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget->ui->durationSpinBox->isReadOnly(),                                    true);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isReadOnly(),                            true);
    // clang-format on
}

TEST_F(Test_SellDecision4ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->enabledCheckBox->setChecked(false);
    configWidget->ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(*sellDecision4ConfigMock, setEnabled(true));
    configWidget->ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget->ui->durationSpinBox->isEnabled(),         true);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isEnabled(), true);
    // clang-format on

    EXPECT_CALL(*sellDecision4ConfigMock, setEnabled(false));
    configWidget->ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget->ui->durationSpinBox->isEnabled(),         false);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isEnabled(), false);
    // clang-format on
}

TEST_F(Test_SellDecision4ConfigWidget, Test_on_durationSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->durationSpinBox->blockSignals(true);
    configWidget->ui->durationSpinBox->setValue(1);
    configWidget->ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(*sellDecision4ConfigMock, setDuration(2));
    configWidget->ui->durationSpinBox->setValue(2);

    EXPECT_CALL(*sellDecision4ConfigMock, setDuration(3));
    configWidget->ui->durationSpinBox->setValue(3);
}

TEST_F(Test_SellDecision4ConfigWidget, Test_on_yieldAboveDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->yieldAboveDoubleSpinBox->blockSignals(true);
    configWidget->ui->yieldAboveDoubleSpinBox->setValue(1.0f);
    configWidget->ui->yieldAboveDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*sellDecision4ConfigMock, setYieldAbove(2.0f));
    configWidget->ui->yieldAboveDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*sellDecision4ConfigMock, setYieldAbove(3.0f));
    configWidget->ui->yieldAboveDoubleSpinBox->setValue(3.0f);
}
// NOLINTEND(readability-magic-numbers)
