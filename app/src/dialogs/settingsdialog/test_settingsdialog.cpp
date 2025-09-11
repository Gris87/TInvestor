#include "src/dialogs/settingsdialog/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidgetfactory_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_SettingsDialog : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        configMock                           = new StrictMock<ConfigMock>();
        simulatorConfigMock                  = new StrictMock<DecisionMakerConfigMock>();
        autoPilotConfigMock                  = new StrictMock<DecisionMakerConfigMock>();
        simulatorConfigWidgetMock            = new StrictMock<DecisionMakerConfigWidgetMock>();
        autoPilotConfigWidgetMock            = new StrictMock<DecisionMakerConfigWidgetMock>();
        decisionMakerConfigWidgetFactoryMock = new StrictMock<DecisionMakerConfigWidgetFactoryMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        buyDecision4ConfigWidgetFactoryMock  = new StrictMock<BuyDecision4ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();
        sellDecision4ConfigWidgetFactoryMock = new StrictMock<SellDecision4ConfigWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();

        EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
        EXPECT_CALL(
            *decisionMakerConfigWidgetFactoryMock,
            newInstance(
                simulatorConfigMock,
                buyDecision1ConfigWidgetFactoryMock,
                buyDecision2ConfigWidgetFactoryMock,
                buyDecision3ConfigWidgetFactoryMock,
                buyDecision4ConfigWidgetFactoryMock,
                sellDecision1ConfigWidgetFactoryMock,
                sellDecision2ConfigWidgetFactoryMock,
                sellDecision3ConfigWidgetFactoryMock,
                sellDecision4ConfigWidgetFactoryMock,
                NotNull()
            )
        )
            .WillOnce(Return(simulatorConfigWidgetMock));

        EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
        EXPECT_CALL(
            *decisionMakerConfigWidgetFactoryMock,
            newInstance(
                autoPilotConfigMock,
                buyDecision1ConfigWidgetFactoryMock,
                buyDecision2ConfigWidgetFactoryMock,
                buyDecision3ConfigWidgetFactoryMock,
                buyDecision4ConfigWidgetFactoryMock,
                sellDecision1ConfigWidgetFactoryMock,
                sellDecision2ConfigWidgetFactoryMock,
                sellDecision3ConfigWidgetFactoryMock,
                sellDecision4ConfigWidgetFactoryMock,
                NotNull()
            )
        )
            .WillOnce(Return(autoPilotConfigWidgetMock));

        EXPECT_CALL(*userStorageMock, readLock());
        EXPECT_CALL(*userStorageMock, getCommission()).WillOnce(Return(0.04));
        EXPECT_CALL(*userStorageMock, readUnlock());

        dialog = new SettingsDialog(
            configMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            buyDecision4ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            sellDecision4ConfigWidgetFactoryMock,
            userStorageMock
        );
    }

    void TearDown() override
    {
        delete dialog;
        delete configMock;
        delete simulatorConfigMock;
        delete autoPilotConfigMock;
        // It will be deleted by `delete ui;`
        /*
        delete simulatorConfigWidgetMock;
        delete autoPilotConfigWidgetMock;
        */
        delete decisionMakerConfigWidgetFactoryMock;
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete buyDecision4ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
        delete sellDecision4ConfigWidgetFactoryMock;
        delete userStorageMock;
    }

    SettingsDialog*                                   dialog;
    StrictMock<ConfigMock>*                           configMock;
    StrictMock<DecisionMakerConfigMock>*              simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock>*              autoPilotConfigMock;
    StrictMock<DecisionMakerConfigWidgetMock>*        simulatorConfigWidgetMock;
    StrictMock<DecisionMakerConfigWidgetMock>*        autoPilotConfigWidgetMock;
    StrictMock<DecisionMakerConfigWidgetFactoryMock>* decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>*  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>*  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>*  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<BuyDecision4ConfigWidgetFactoryMock>*  buyDecision4ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock>* sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock>* sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock>* sellDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision4ConfigWidgetFactoryMock>* sellDecision4ConfigWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
};



TEST_F(Test_SettingsDialog, Test_constructor_and_destructor)
{
}

TEST_F(Test_SettingsDialog, Test_updateUiFromConfig)
{
    const InSequence seq;

    dialog->ui->autorunCheckBox->blockSignals(true);
    dialog->ui->cpuUsageComboBox->blockSignals(true);
    dialog->ui->makeDecisionTimeoutSpinBox->blockSignals(true);
    dialog->ui->tradeInNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->tradeHugeSpreadCheckBox->blockSignals(true);
    dialog->ui->hugeSpreadDoubleSpinBox->blockSignals(true);
    dialog->ui->tradeLiquidityEtfNightlyCheckBox->blockSignals(true);
    dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->blockSignals(true);
    dialog->ui->limitStockPurchaseCheckBox->blockSignals(true);
    dialog->ui->limitStockPurchasePartDoubleSpinBox->blockSignals(true);
    dialog->ui->limitByTurnoverCheckBox->blockSignals(true);
    dialog->ui->limitByTurnoverPercentDoubleSpinBox->blockSignals(true);
    dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->blockSignals(true);
    dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->blockSignals(true);
    dialog->ui->storageMonthLimitSpinBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);

    EXPECT_CALL(*simulatorConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*autoPilotConfigWidgetMock, updateUiFromConfig());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getCpuUsage()).WillOnce(Return("OPTIMAL"));
    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(2));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeHugeSpread()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeSpread()).WillOnce(Return(3.0f));
    EXPECT_CALL(*configMock, isTradeLiquidityEtfNightly()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLiquidityEtfRemainedPartNightly()).WillOnce(Return(7.0f));
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitStockPurchasePart()).WillOnce(Return(20.0f));
    EXPECT_CALL(*configMock, isLimitByTurnover()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitByTurnoverPercent()).WillOnce(Return(1.0f));
    EXPECT_CALL(*configMock, isLimitStockPurchaseNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitStockPurchasePartNonWorkingHours()).WillOnce(Return(15.0f));
    EXPECT_CALL(*configMock, isLimitByTurnoverNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitByTurnoverPercentNonWorkingHours()).WillOnce(Return(8.0f));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(36));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),                                  true);
    ASSERT_EQ(dialog->ui->cpuUsageComboBox->currentIndex(),                              2);
    ASSERT_EQ(dialog->ui->makeDecisionTimeoutSpinBox->value(),                           2);
    ASSERT_EQ(dialog->ui->tradeInNonWorkingHoursCheckBox->isChecked(),                   true);
    ASSERT_EQ(dialog->ui->tradeHugeSpreadCheckBox->isChecked(),                          true);
    ASSERT_NEAR(dialog->ui->hugeSpreadDoubleSpinBox->value(),                            3.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->tradeLiquidityEtfNightlyCheckBox->isChecked(),                 true);
    ASSERT_NEAR(dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->value(),       7.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitStockPurchaseCheckBox->isChecked(),                       true);
    ASSERT_NEAR(dialog->ui->limitStockPurchasePartDoubleSpinBox->value(),                20.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitByTurnoverCheckBox->isChecked(),                          true);
    ASSERT_NEAR(dialog->ui->limitByTurnoverPercentDoubleSpinBox->value(),                1.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->isChecked(),        true);
    ASSERT_NEAR(dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->value(), 15.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->isChecked(),           true);
    ASSERT_NEAR(dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->value(), 8.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->storageMonthLimitSpinBox->value(),                             36);
    ASSERT_EQ(dialog->ui->simulatorConfigCommonCheckBox->isChecked(),                    true);
    ASSERT_EQ(dialog->ui->autoPilotConfigCommonCheckBox->isChecked(),                    false);
    // clang-format on

    EXPECT_CALL(*simulatorConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*autoPilotConfigWidgetMock, updateUiFromConfig());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getCpuUsage()).WillOnce(Return("MINIMUM"));
    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(5));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isTradeHugeSpread()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getHugeSpread()).WillOnce(Return(2.0f));
    EXPECT_CALL(*configMock, isTradeLiquidityEtfNightly()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getLiquidityEtfRemainedPartNightly()).WillOnce(Return(10.0f));
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getLimitStockPurchasePart()).WillOnce(Return(50.0f));
    EXPECT_CALL(*configMock, isLimitByTurnover()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getLimitByTurnoverPercent()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isLimitStockPurchaseNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getLimitStockPurchasePartNonWorkingHours()).WillOnce(Return(30.0f));
    EXPECT_CALL(*configMock, isLimitByTurnoverNonWorkingHours()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getLimitByTurnoverPercentNonWorkingHours()).WillOnce(Return(2.5f));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(12));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(true));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),                                  false);
    ASSERT_EQ(dialog->ui->cpuUsageComboBox->currentIndex(),                              0);
    ASSERT_EQ(dialog->ui->makeDecisionTimeoutSpinBox->value(),                           5);
    ASSERT_EQ(dialog->ui->tradeInNonWorkingHoursCheckBox->isChecked(),                   false);
    ASSERT_EQ(dialog->ui->tradeHugeSpreadCheckBox->isChecked(),                          false);
    ASSERT_NEAR(dialog->ui->hugeSpreadDoubleSpinBox->value(),                            2.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->tradeLiquidityEtfNightlyCheckBox->isChecked(),                 false);
    ASSERT_NEAR(dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->value(),       10.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitStockPurchaseCheckBox->isChecked(),                       false);
    ASSERT_NEAR(dialog->ui->limitStockPurchasePartDoubleSpinBox->value(),                50.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitByTurnoverCheckBox->isChecked(),                          false);
    ASSERT_NEAR(dialog->ui->limitByTurnoverPercentDoubleSpinBox->value(),                5.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->isChecked(),        false);
    ASSERT_NEAR(dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->value(), 30.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->isChecked(),           false);
    ASSERT_NEAR(dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->value(), 2.5f, 0.0001f);
    ASSERT_EQ(dialog->ui->storageMonthLimitSpinBox->value(),                             12);
    ASSERT_EQ(dialog->ui->simulatorConfigCommonCheckBox->isChecked(),                    false);
    ASSERT_EQ(dialog->ui->autoPilotConfigCommonCheckBox->isChecked(),                    true);
    // clang-format on
}

TEST_F(Test_SettingsDialog, Test_on_autorunCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->autorunCheckBox->blockSignals(true);
    dialog->ui->autorunCheckBox->setChecked(false);
    dialog->ui->autorunCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setAutorun(true));
    dialog->ui->autorunCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setAutorun(false));
    dialog->ui->autorunCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_cpuUsageComboBox_currentIndexChanged)
{
    const InSequence seq;

    dialog->ui->cpuUsageComboBox->blockSignals(true);
    dialog->ui->cpuUsageComboBox->setCurrentIndex(0);
    dialog->ui->cpuUsageComboBox->blockSignals(false);

    EXPECT_CALL(*configMock, setCpuUsage(QString("NORMAL")));
    dialog->ui->cpuUsageComboBox->setCurrentIndex(1);

    EXPECT_CALL(*configMock, setCpuUsage(QString("OPTIMAL")));
    dialog->ui->cpuUsageComboBox->setCurrentIndex(2);
}

TEST_F(Test_SettingsDialog, Test_on_makeDecisionTimeoutSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->makeDecisionTimeoutSpinBox->blockSignals(true);
    dialog->ui->makeDecisionTimeoutSpinBox->setValue(1);
    dialog->ui->makeDecisionTimeoutSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setMakeDecisionTimeout(2));
    dialog->ui->makeDecisionTimeoutSpinBox->setValue(2);

    EXPECT_CALL(*configMock, setMakeDecisionTimeout(3));
    dialog->ui->makeDecisionTimeoutSpinBox->setValue(3);
}

TEST_F(Test_SettingsDialog, Test_on_tradeInNonWorkingHoursCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->tradeInNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->tradeInNonWorkingHoursCheckBox->setChecked(false);
    dialog->ui->tradeInNonWorkingHoursCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setTradeInNonWorkingHours(true));
    dialog->ui->tradeInNonWorkingHoursCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->limitStockPurchaseNonWorkingHoursWidget->isEnabled(), true);

    EXPECT_CALL(*configMock, setTradeInNonWorkingHours(false));
    dialog->ui->tradeInNonWorkingHoursCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->limitStockPurchaseNonWorkingHoursWidget->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_tradeHugeSpreadCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->tradeHugeSpreadCheckBox->blockSignals(true);
    dialog->ui->tradeHugeSpreadCheckBox->setChecked(false);
    dialog->ui->tradeHugeSpreadCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setTradeHugeSpread(true));
    dialog->ui->tradeHugeSpreadCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->hugeSpreadDoubleSpinBox->isEnabled(), true);

    EXPECT_CALL(*configMock, setTradeHugeSpread(false));
    dialog->ui->tradeHugeSpreadCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->hugeSpreadDoubleSpinBox->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_hugeSpreadDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->hugeSpreadDoubleSpinBox->blockSignals(true);
    dialog->ui->hugeSpreadDoubleSpinBox->setValue(1.0f);
    dialog->ui->hugeSpreadDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setHugeSpread(2.0f));
    dialog->ui->hugeSpreadDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*configMock, setHugeSpread(3.0f));
    dialog->ui->hugeSpreadDoubleSpinBox->setValue(3.0f);
}

TEST_F(Test_SettingsDialog, Test_on_tradeLiquidityEtfNightlyCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->tradeLiquidityEtfNightlyCheckBox->blockSignals(true);
    dialog->ui->tradeLiquidityEtfNightlyCheckBox->setChecked(false);
    dialog->ui->tradeLiquidityEtfNightlyCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setTradeLiquidityEtfNightly(true));
    dialog->ui->tradeLiquidityEtfNightlyCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->isEnabled(), true);

    EXPECT_CALL(*configMock, setTradeLiquidityEtfNightly(false));
    dialog->ui->tradeLiquidityEtfNightlyCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_liquidityEtfRemainedPartNightlyDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->blockSignals(true);
    dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->setValue(51);
    dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLiquidityEtfRemainedPartNightly(12.0f));
    dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->setValue(12.0f);

    EXPECT_CALL(*configMock, setLiquidityEtfRemainedPartNightly(13.0f));
    dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->setValue(13.0f);
}

TEST_F(Test_SettingsDialog, Test_on_limitStockPurchaseCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->limitStockPurchaseCheckBox->blockSignals(true);
    dialog->ui->limitStockPurchaseCheckBox->setChecked(false);
    dialog->ui->limitStockPurchaseCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitStockPurchase(true));
    dialog->ui->limitStockPurchaseCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->limitByTurnoverWidget->isEnabled(), true);

    EXPECT_CALL(*configMock, setLimitStockPurchase(false));
    dialog->ui->limitStockPurchaseCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->limitByTurnoverWidget->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_limitStockPurchasePartDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->limitStockPurchasePartDoubleSpinBox->blockSignals(true);
    dialog->ui->limitStockPurchasePartDoubleSpinBox->setValue(1.0f);
    dialog->ui->limitStockPurchasePartDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitStockPurchasePart(2.0f));
    dialog->ui->limitStockPurchasePartDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*configMock, setLimitStockPurchasePart(3.0f));
    dialog->ui->limitStockPurchasePartDoubleSpinBox->setValue(3.0f);
}

TEST_F(Test_SettingsDialog, Test_on_limitByTurnoverCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->limitByTurnoverCheckBox->blockSignals(true);
    dialog->ui->limitByTurnoverCheckBox->setChecked(false);
    dialog->ui->limitByTurnoverCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitByTurnover(true));
    dialog->ui->limitByTurnoverCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->limitByTurnoverPercentDoubleSpinBox->isEnabled(), true);

    EXPECT_CALL(*configMock, setLimitByTurnover(false));
    dialog->ui->limitByTurnoverCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->limitByTurnoverPercentDoubleSpinBox->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_limitByTurnoverPercentDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->limitByTurnoverPercentDoubleSpinBox->blockSignals(true);
    dialog->ui->limitByTurnoverPercentDoubleSpinBox->setValue(1);
    dialog->ui->limitByTurnoverPercentDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitByTurnoverPercent(2.0f));
    dialog->ui->limitByTurnoverPercentDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*configMock, setLimitByTurnoverPercent(3.0f));
    dialog->ui->limitByTurnoverPercentDoubleSpinBox->setValue(3.0f);
}

TEST_F(Test_SettingsDialog, Test_on_limitStockPurchaseNonWorkingHoursCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->setChecked(false);
    dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitStockPurchaseNonWorkingHours(true));
    dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->limitByTurnoverNonWorkingHoursWidget->isEnabled(), true);

    EXPECT_CALL(*configMock, setLimitStockPurchaseNonWorkingHours(false));
    dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->limitByTurnoverNonWorkingHoursWidget->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_limitStockPurchasePartNonWorkingHoursDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->blockSignals(true);
    dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->setValue(1.0f);
    dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitStockPurchasePartNonWorkingHours(2.0f));
    dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*configMock, setLimitStockPurchasePartNonWorkingHours(3.0f));
    dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->setValue(3.0f);
}

TEST_F(Test_SettingsDialog, Test_on_limitByTurnoverNonWorkingHoursCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->setChecked(false);
    dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitByTurnoverNonWorkingHours(true));
    dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->isEnabled(), true);

    EXPECT_CALL(*configMock, setLimitByTurnoverNonWorkingHours(false));
    dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_limitByTurnoverPercentNonWorkingHoursDoubleSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->blockSignals(true);
    dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->setValue(1);
    dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitByTurnoverPercentNonWorkingHours(2.0f));
    dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->setValue(2.0f);

    EXPECT_CALL(*configMock, setLimitByTurnoverPercentNonWorkingHours(3.0f));
    dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->setValue(3.0f);
}

TEST_F(Test_SettingsDialog, Test_on_storageMonthLimitSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->storageMonthLimitSpinBox->blockSignals(true);
    dialog->ui->storageMonthLimitSpinBox->setValue(1);
    dialog->ui->storageMonthLimitSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setStorageMonthLimit(2));
    dialog->ui->storageMonthLimitSpinBox->setValue(2);

    EXPECT_CALL(*configMock, setStorageMonthLimit(3));
    dialog->ui->storageMonthLimitSpinBox->setValue(3);
}

TEST_F(Test_SettingsDialog, Test_on_simulatorConfigCommonCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(false);
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(false);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(false);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(false);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(true));
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Decision maker");

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(false));
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(false);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(2), "Auto-pilot");
}

TEST_F(Test_SettingsDialog, Test_on_simulatorConfigCommonCheckBox_checkStateChanged_unexpected_behaviour)
{
    const InSequence seq;

    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(false);
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(false);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(false);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(false);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(true));
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(true);
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Decision maker");
}

TEST_F(Test_SettingsDialog, Test_on_autoPilotConfigCommonCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(false);
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(false);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(false);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(false);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(true));
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Decision maker");

    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(false));
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(false);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(2), "Auto-pilot");
}

TEST_F(Test_SettingsDialog, Test_on_autoPilotConfigCommonCheckBox_checkStateChanged_unexpected_behaviour)
{
    const InSequence seq;

    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(false);
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(false);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(false);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(false);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(true));
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(true);
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->tabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->tabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->tabWidget->tabText(1), "Decision maker");
}

TEST_F(Test_SettingsDialog, Test_on_okButton_clicked)
{
    ASSERT_EQ(dialog->result(), 0);
    dialog->ui->okButton->click();
    ASSERT_EQ(dialog->result(), QDialog::Accepted);
}

TEST_F(Test_SettingsDialog, Test_on_cancelButton_clicked)
{
    ASSERT_EQ(dialog->result(), 0);
    dialog->ui->cancelButton->click();
    ASSERT_EQ(dialog->result(), QDialog::Rejected);
}

TEST_F(Test_SettingsDialog, Test_on_defaultButton_clicked)
{
    const InSequence seq;

    dialog->ui->autorunCheckBox->blockSignals(true);
    dialog->ui->cpuUsageComboBox->blockSignals(true);
    dialog->ui->makeDecisionTimeoutSpinBox->blockSignals(true);
    dialog->ui->tradeInNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->tradeHugeSpreadCheckBox->blockSignals(true);
    dialog->ui->hugeSpreadDoubleSpinBox->blockSignals(true);
    dialog->ui->tradeLiquidityEtfNightlyCheckBox->blockSignals(true);
    dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->blockSignals(true);
    dialog->ui->limitStockPurchaseCheckBox->blockSignals(true);
    dialog->ui->limitStockPurchasePartDoubleSpinBox->blockSignals(true);
    dialog->ui->limitByTurnoverCheckBox->blockSignals(true);
    dialog->ui->limitByTurnoverPercentDoubleSpinBox->blockSignals(true);
    dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->blockSignals(true);
    dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->blockSignals(true);
    dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->blockSignals(true);
    dialog->ui->storageMonthLimitSpinBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);

    EXPECT_CALL(*configMock, makeDefault());

    EXPECT_CALL(*simulatorConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*autoPilotConfigWidgetMock, updateUiFromConfig());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getCpuUsage()).WillOnce(Return("OPTIMAL"));
    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(2));
    EXPECT_CALL(*configMock, isTradeInNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isTradeHugeSpread()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getHugeSpread()).WillOnce(Return(1.0f));
    EXPECT_CALL(*configMock, isTradeLiquidityEtfNightly()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLiquidityEtfRemainedPartNightly()).WillOnce(Return(5.0f));
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitStockPurchasePart()).WillOnce(Return(20.0f));
    EXPECT_CALL(*configMock, isLimitByTurnover()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitByTurnoverPercent()).WillOnce(Return(1.0f));
    EXPECT_CALL(*configMock, isLimitStockPurchaseNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitStockPurchasePartNonWorkingHours()).WillOnce(Return(15.0f));
    EXPECT_CALL(*configMock, isLimitByTurnoverNonWorkingHours()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getLimitByTurnoverPercentNonWorkingHours()).WillOnce(Return(3.0f));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(36));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));

    dialog->ui->defaultButton->click();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),                                  true);
    ASSERT_EQ(dialog->ui->cpuUsageComboBox->currentIndex(),                              2);
    ASSERT_EQ(dialog->ui->makeDecisionTimeoutSpinBox->value(),                           2);
    ASSERT_EQ(dialog->ui->tradeInNonWorkingHoursCheckBox->isChecked(),                   true);
    ASSERT_EQ(dialog->ui->tradeHugeSpreadCheckBox->isChecked(),                          true);
    ASSERT_NEAR(dialog->ui->hugeSpreadDoubleSpinBox->value(),                            1.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->tradeLiquidityEtfNightlyCheckBox->isChecked(),                 true);
    ASSERT_NEAR(dialog->ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->value(),       5.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitStockPurchaseCheckBox->isChecked(),                       true);
    ASSERT_NEAR(dialog->ui->limitStockPurchasePartDoubleSpinBox->value(),                20.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitByTurnoverCheckBox->isChecked(),                          true);
    ASSERT_NEAR(dialog->ui->limitByTurnoverPercentDoubleSpinBox->value(),                1.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitStockPurchaseNonWorkingHoursCheckBox->isChecked(),        true);
    ASSERT_NEAR(dialog->ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->value(), 15.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->limitByTurnoverNonWorkingHoursCheckBox->isChecked(),           true);
    ASSERT_NEAR(dialog->ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->value(), 3.0f, 0.0001f);
    ASSERT_EQ(dialog->ui->storageMonthLimitSpinBox->value(),                             36);
    ASSERT_EQ(dialog->ui->simulatorConfigCommonCheckBox->isChecked(),                    true);
    ASSERT_EQ(dialog->ui->autoPilotConfigCommonCheckBox->isChecked(),                    false);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
