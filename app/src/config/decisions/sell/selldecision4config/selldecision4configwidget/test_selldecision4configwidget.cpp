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
    configWidget->ui->loseYieldDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(*sellDecision4ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*sellDecision4ConfigMock, getLoseYield()).WillOnce(Return(3.1f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),      true);
    ASSERT_NEAR(configWidget->ui->loseYieldDoubleSpinBox->value(), 3.1f, 0.0001f);
    // clang-format on

    EXPECT_CALL(*sellDecision4ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(*sellDecision4ConfigMock, getLoseYield()).WillOnce(Return(6.3f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(),      false);
    ASSERT_NEAR(configWidget->ui->loseYieldDoubleSpinBox->value(), 6.3f, 0.0001f);
    // clang-format on
}

TEST_F(Test_SellDecision4ConfigWidget, Test_makeReadOnly)
{
    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget->ui->loseYieldDoubleSpinBox->isReadOnly(),                             false);
    // clang-format on

    configWidget->makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget->ui->loseYieldDoubleSpinBox->isReadOnly(),                             true);
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
    ASSERT_EQ(configWidget->ui->loseYieldDoubleSpinBox->isEnabled(), true);
    // clang-format on

    EXPECT_CALL(*sellDecision4ConfigMock, setEnabled(false));
    configWidget->ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget->ui->loseYieldDoubleSpinBox->isEnabled(), false);
    // clang-format on
}

TEST_F(Test_SellDecision4ConfigWidget, Test_on_loseYieldDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->loseYieldDoubleSpinBox->blockSignals(true);
    configWidget->ui->loseYieldDoubleSpinBox->setValue(1.0f);
    configWidget->ui->loseYieldDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*sellDecision4ConfigMock, setLoseYield(2.0f));
    configWidget->ui->loseYieldDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*sellDecision4ConfigMock, setLoseYield(3.0f));
    configWidget->ui->loseYieldDoubleSpinBox->setValue(3.0f);
}
// NOLINTEND(readability-magic-numbers)
