#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/buydecision9configwidget.h"
#include "ui_buydecision9configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision9config/ibuydecision9config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
class Test_BuyDecision9ConfigWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision9ConfigMock = new StrictMock<BuyDecision9ConfigMock>();

        configWidget = new BuyDecision9ConfigWidget(buyDecision9ConfigMock);
    }

    void TearDown() override
    {
        delete configWidget;
        delete buyDecision9ConfigMock;
    }

    BuyDecision9ConfigWidget*           configWidget;
    StrictMock<BuyDecision9ConfigMock>* buyDecision9ConfigMock;
};



TEST_F(Test_BuyDecision9ConfigWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->rsiDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(*buyDecision9ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsi()).WillOnce(Return(4.1f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(), true);
    ASSERT_NEAR(configWidget->ui->rsiDoubleSpinBox->value(),  4.1f, 0.0001f);
    // clang-format on

    EXPECT_CALL(*buyDecision9ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(*buyDecision9ConfigMock, getRsi()).WillOnce(Return(7.3f));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(), false);
    ASSERT_NEAR(configWidget->ui->rsiDoubleSpinBox->value(),  7.3f, 0.0001f);
    // clang-format on
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_makeReadOnly)
{
    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget->ui->rsiDoubleSpinBox->isReadOnly(),                                   false);
    // clang-format on

    configWidget->makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget->ui->rsiDoubleSpinBox->isReadOnly(),                                   true);
    // clang-format on
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->enabledCheckBox->setChecked(false);
    configWidget->ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(*buyDecision9ConfigMock, setEnabled(true));
    configWidget->ui->enabledCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(configWidget->ui->rsiDoubleSpinBox->isEnabled(), true);
    // clang-format on

    EXPECT_CALL(*buyDecision9ConfigMock, setEnabled(false));
    configWidget->ui->enabledCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(configWidget->ui->rsiDoubleSpinBox->isEnabled(), false);
    // clang-format on
}

TEST_F(Test_BuyDecision9ConfigWidget, Test_on_rsiDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->rsiDoubleSpinBox->blockSignals(true);
    configWidget->ui->rsiDoubleSpinBox->setValue(1.0f);
    configWidget->ui->rsiDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsi(2.0f));
    configWidget->ui->rsiDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*buyDecision9ConfigMock, setRsi(3.0f));
    configWidget->ui->rsiDoubleSpinBox->setValue(3.0f);
}
// NOLINTEND(readability-magic-numbers)
