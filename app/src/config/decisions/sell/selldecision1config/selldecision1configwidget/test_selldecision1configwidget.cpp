#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidget.h"
#include "ui_selldecision1configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
class Test_SellDecision1ConfigWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision1ConfigMock = new StrictMock<SellDecision1ConfigMock>();

        configWidget = new SellDecision1ConfigWidget(sellDecision1ConfigMock);
    }

    void TearDown() override
    {
        delete configWidget;
        delete sellDecision1ConfigMock;
    }

    SellDecision1ConfigWidget*           configWidget;
    StrictMock<SellDecision1ConfigMock>* sellDecision1ConfigMock;
};



TEST_F(Test_SellDecision1ConfigWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision1ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->yieldAboveDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(*sellDecision1ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision1ConfigMock, getYieldAbove()).WillOnce(Return(2.1f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),       true);
    ASSERT_NEAR(configWidget->ui->yieldAboveDoubleSpinBox->value(), 2.1f, 0.0001f);
    // clang-format on

    EXPECT_CALL(*sellDecision1ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(*sellDecision1ConfigMock, getYieldAbove()).WillOnce(Return(5.3f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),       false);
    ASSERT_NEAR(configWidget->ui->yieldAboveDoubleSpinBox->value(), 5.3f, 0.0001f);
    // clang-format on
}

TEST_F(Test_SellDecision1ConfigWidget, Test_makeReadOnly)
{
    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isReadOnly(),                            false);
    // clang-format on

    configWidget->makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isReadOnly(),                            true);
    // clang-format on
}

TEST_F(Test_SellDecision1ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->enabledCheckBox->setChecked(false);
    configWidget->ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(*sellDecision1ConfigMock, setEnabled(true));
    configWidget->ui->enabledCheckBox->setChecked(true);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isEnabled(), true);

    EXPECT_CALL(*sellDecision1ConfigMock, setEnabled(false));
    configWidget->ui->enabledCheckBox->setChecked(false);
    ASSERT_EQ(configWidget->ui->yieldAboveDoubleSpinBox->isEnabled(), false);
}

TEST_F(Test_SellDecision1ConfigWidget, Test_on_yieldAboveDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->yieldAboveDoubleSpinBox->blockSignals(true);
    configWidget->ui->yieldAboveDoubleSpinBox->setValue(1.0f);
    configWidget->ui->yieldAboveDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*sellDecision1ConfigMock, setYieldAbove(2.0f));
    configWidget->ui->yieldAboveDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*sellDecision1ConfigMock, setYieldAbove(3.0f));
    configWidget->ui->yieldAboveDoubleSpinBox->setValue(3.0f);
}
// NOLINTEND(readability-magic-numbers)
