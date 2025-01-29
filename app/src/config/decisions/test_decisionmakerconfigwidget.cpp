#include <gtest/gtest.h>

#include "src/config/decisions/decisionmakerconfigwidget.h"
#include "src/config/decisions/buy/ibuydecision1config_mock.h"
#include "src/config/decisions/buy/ibuydecision2config_mock.h"
#include "src/config/decisions/buy/ibuydecision3config_mock.h"
#include "src/config/decisions/sell/iselldecision1config_mock.h"
#include "src/config/decisions/sell/iselldecision2config_mock.h"
#include "src/config/decisions/sell/iselldecision3config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "ui_buydecision1configwidget.h"
#include "ui_buydecision2configwidget.h"
#include "ui_buydecision3configwidget.h"
#include "ui_decisionmakerconfigwidget.h"
#include "ui_selldecision1configwidget.h"
#include "ui_selldecision2configwidget.h"
#include "ui_selldecision3configwidget.h"



using ::testing::StrictMock;
using ::testing::Return;



TEST(Test_DecisionMakerConfigWidget, Test_constructor_and_destructor)
{
    DecisionMakerConfigWidget configWidget;
}

TEST(Test_DecisionMakerConfigWidget, Test_setDecisionMakerConfig)
{
    DecisionMakerConfigWidget configWidget;

    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;
    StrictMock<DecisionMakerConfigMock> decisionMakerConfigMock;

    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision1Config()).WillOnce(Return(&buyDecision1ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision2Config()).WillOnce(Return(&buyDecision2ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision3Config()).WillOnce(Return(&buyDecision3ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision1Config()).WillOnce(Return(&sellDecision1ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision2Config()).WillOnce(Return(&sellDecision2ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision3Config()).WillOnce(Return(&sellDecision3ConfigMock));

    configWidget.setDecisionMakerConfig(&decisionMakerConfigMock);
}

TEST(Test_DecisionMakerConfigWidget, Test_updateUiFromConfig)
{
    DecisionMakerConfigWidget configWidget;

    StrictMock<BuyDecision1ConfigMock>  buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;
    StrictMock<DecisionMakerConfigMock> decisionMakerConfigMock;

    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision1Config()).WillOnce(Return(&buyDecision1ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision2Config()).WillOnce(Return(&buyDecision2ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision3Config()).WillOnce(Return(&buyDecision3ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision1Config()).WillOnce(Return(&sellDecision1ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision2Config()).WillOnce(Return(&sellDecision2ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision3Config()).WillOnce(Return(&sellDecision3ConfigMock));

    configWidget.setDecisionMakerConfig(&decisionMakerConfigMock);

    configWidget.ui->buyDecision1ConfigWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->buyDecision1ConfigWidget->ui->priceFallDoubleSpinBox->blockSignals(true);
    configWidget.ui->buyDecision1ConfigWidget->ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(buyDecision1ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(buyDecision1ConfigMock, getPriceFall()).WillOnce(Return(2.1f));
    EXPECT_CALL(buyDecision1ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget.ui->buyDecision2ConfigWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->buyDecision2ConfigWidget->ui->priceDiffDoubleSpinBox->blockSignals(true);
    configWidget.ui->buyDecision2ConfigWidget->ui->amountOfTimesSpinBox->blockSignals(true);
    configWidget.ui->buyDecision2ConfigWidget->ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(buyDecision2ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(buyDecision2ConfigMock, getPriceDiff()).WillOnce(Return(2.1f));
    EXPECT_CALL(buyDecision2ConfigMock, getAmountOfTimes()).WillOnce(Return(1));
    EXPECT_CALL(buyDecision2ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget.ui->buyDecision3ConfigWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->buyDecision3ConfigWidget->ui->priceRiseDoubleSpinBox->blockSignals(true);
    configWidget.ui->buyDecision3ConfigWidget->ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(buyDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(buyDecision3ConfigMock, getPriceRise()).WillOnce(Return(2.1f));
    EXPECT_CALL(buyDecision3ConfigMock, getDuration()).WillOnce(Return(3));

    configWidget.ui->sellDecision1ConfigWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->sellDecision1ConfigWidget->ui->incomeAboveDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision1ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision1ConfigMock, getIncomeAbove()).WillOnce(Return(2.1f));

    configWidget.ui->sellDecision2ConfigWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->sellDecision2ConfigWidget->ui->incomeAboveDoubleSpinBox->blockSignals(true);
    configWidget.ui->sellDecision2ConfigWidget->ui->loseIncomeDoubleSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision2ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision2ConfigMock, getIncomeAbove()).WillOnce(Return(2.1f));
    EXPECT_CALL(sellDecision2ConfigMock, getLoseIncome()).WillOnce(Return(3.1f));

    configWidget.ui->sellDecision3ConfigWidget->ui->enabledCheckBox->blockSignals(true);
    configWidget.ui->sellDecision3ConfigWidget->ui->loseIncomeDoubleSpinBox->blockSignals(true);
    configWidget.ui->sellDecision3ConfigWidget->ui->durationSpinBox->blockSignals(true);

    EXPECT_CALL(sellDecision3ConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(sellDecision3ConfigMock, getLoseIncome()).WillOnce(Return(2.1f));
    EXPECT_CALL(sellDecision3ConfigMock, getDuration()).WillOnce(Return(4));

    configWidget.updateUiFromConfig();
}
