#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/buydecision5configwidget.h"
#include "ui_buydecision5configwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision5config/ibuydecision5config_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
class Test_BuyDecision5ConfigWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision5ConfigMock = new StrictMock<BuyDecision5ConfigMock>();

        configWidget = new BuyDecision5ConfigWidget(buyDecision5ConfigMock);
    }

    void TearDown() override
    {
        delete configWidget;
        delete buyDecision5ConfigMock;
    }

    BuyDecision5ConfigWidget*           configWidget;
    StrictMock<BuyDecision5ConfigMock>* buyDecision5ConfigMock;
};



TEST_F(Test_BuyDecision5ConfigWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision5ConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(*buyDecision5ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*buyDecision5ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(), true);
    ASSERT_EQ(configWidget->ui->durationSpinBox->value(),     3);
    // clang-format on

    EXPECT_CALL(*buyDecision5ConfigMock, isEnabled()).WillOnce(Return(false));
    EXPECT_CALL(*buyDecision5ConfigMock, getDuration()).WillOnce(Return(2));

    configWidget->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->isChecked(), false);
    ASSERT_EQ(configWidget->ui->durationSpinBox->value(),     2);
    // clang-format on
}

TEST_F(Test_BuyDecision5ConfigWidget, Test_makeReadOnly)
{
    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), false);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::StrongFocus);
    ASSERT_EQ(configWidget->ui->durationSpinBox->isReadOnly(),                                    false);
    // clang-format on

    configWidget->makeReadOnly();

    // clang-format off
    ASSERT_EQ(configWidget->ui->enabledCheckBox->testAttribute(Qt::WA_TransparentForMouseEvents), true);
    ASSERT_EQ(configWidget->ui->enabledCheckBox->focusPolicy(),                                   Qt::NoFocus);
    ASSERT_EQ(configWidget->ui->durationSpinBox->isReadOnly(),                                    true);
    // clang-format on
}

TEST_F(Test_BuyDecision5ConfigWidget, Test_on_enabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    configWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget->ui->enabledCheckBox->setChecked(false);
    configWidget->ui->enabledCheckBox->blockSignals(false);

    EXPECT_CALL(*buyDecision5ConfigMock, setEnabled(true));
    configWidget->ui->enabledCheckBox->setChecked(true);

    ASSERT_EQ(configWidget->ui->durationSpinBox->isEnabled(), true);

    EXPECT_CALL(*buyDecision5ConfigMock, setEnabled(false));
    configWidget->ui->enabledCheckBox->setChecked(false);

    ASSERT_EQ(configWidget->ui->durationSpinBox->isEnabled(), false);
}

TEST_F(Test_BuyDecision5ConfigWidget, Test_on_durationSpinBox_valueChanged)
{
    const InSequence seq;

    configWidget->ui->durationSpinBox->blockSignals(true);
    configWidget->ui->durationSpinBox->setValue(1);
    configWidget->ui->durationSpinBox->blockSignals(false);

    EXPECT_CALL(*buyDecision5ConfigMock, setDuration(2));
    configWidget->ui->durationSpinBox->setValue(2);

    EXPECT_CALL(*buyDecision5ConfigMock, setDuration(3));
    configWidget->ui->durationSpinBox->setValue(3);
}
// NOLINTEND(readability-magic-numbers)
