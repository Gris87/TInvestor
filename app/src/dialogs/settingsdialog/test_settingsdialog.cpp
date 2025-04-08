#include "src/dialogs/settingsdialog/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



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
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();

        EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
        EXPECT_CALL(
            *decisionMakerConfigWidgetFactoryMock,
            newInstance(
                simulatorConfigMock,
                buyDecision1ConfigWidgetFactoryMock,
                buyDecision2ConfigWidgetFactoryMock,
                buyDecision3ConfigWidgetFactoryMock,
                sellDecision1ConfigWidgetFactoryMock,
                sellDecision2ConfigWidgetFactoryMock,
                sellDecision3ConfigWidgetFactoryMock,
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
                sellDecision1ConfigWidgetFactoryMock,
                sellDecision2ConfigWidgetFactoryMock,
                sellDecision3ConfigWidgetFactoryMock,
                NotNull()
            )
        )
            .WillOnce(Return(autoPilotConfigWidgetMock));

        dialog = new SettingsDialog(
            configMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock
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
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
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
    StrictMock<SellDecision1ConfigWidgetFactoryMock>* sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock>* sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock>* sellDecision3ConfigWidgetFactoryMock;
};



TEST_F(Test_SettingsDialog, Test_constructor_and_destructor)
{
}

TEST_F(Test_SettingsDialog, Test_updateUiFromConfig)
{
    const InSequence seq;

    dialog->ui->autorunCheckBox->blockSignals(true);
    dialog->ui->makeDecisionTimeoutSpinBox->blockSignals(true);
    dialog->ui->useScheduleCheckBox->blockSignals(true);
    dialog->ui->scheduleStartTimeEdit->blockSignals(true);
    dialog->ui->scheduleEndTimeEdit->blockSignals(true);
    dialog->ui->limitStockPurchaseCheckBox->blockSignals(true);
    dialog->ui->amountOfStockPurchaseSpinBox->blockSignals(true);
    dialog->ui->storageMonthLimitSpinBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);

    EXPECT_CALL(*configMock, getScheduleStartHour()).WillOnce(Return(10));
    EXPECT_CALL(*configMock, getScheduleStartMinute()).WillOnce(Return(30));
    EXPECT_CALL(*configMock, getScheduleEndHour()).WillOnce(Return(19));
    EXPECT_CALL(*configMock, getScheduleEndMinute()).WillOnce(Return(15));

    EXPECT_CALL(*simulatorConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*autoPilotConfigWidgetMock, updateUiFromConfig());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(2));
    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAmountOfStockPurchase()).WillOnce(Return(20000));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(36));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),               true);
    ASSERT_EQ(dialog->ui->makeDecisionTimeoutSpinBox->value(),        2);
    ASSERT_EQ(dialog->ui->useScheduleCheckBox->isChecked(),           true);
    ASSERT_EQ(dialog->ui->scheduleStartTimeEdit->time(),              QTime(10, 30));
    ASSERT_EQ(dialog->ui->scheduleEndTimeEdit->time(),                QTime(19, 15));
    ASSERT_EQ(dialog->ui->limitStockPurchaseCheckBox->isChecked(),    true);
    ASSERT_EQ(dialog->ui->amountOfStockPurchaseSpinBox->value(),      20000);
    ASSERT_EQ(dialog->ui->storageMonthLimitSpinBox->value(),          36);
    ASSERT_EQ(dialog->ui->simulatorConfigCommonCheckBox->isChecked(), true);
    ASSERT_EQ(dialog->ui->autoPilotConfigCommonCheckBox->isChecked(), false);
    // clang-format on

    EXPECT_CALL(*configMock, getScheduleStartHour()).WillOnce(Return(11));
    EXPECT_CALL(*configMock, getScheduleStartMinute()).WillOnce(Return(15));
    EXPECT_CALL(*configMock, getScheduleEndHour()).WillOnce(Return(20));
    EXPECT_CALL(*configMock, getScheduleEndMinute()).WillOnce(Return(40));

    EXPECT_CALL(*simulatorConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*autoPilotConfigWidgetMock, updateUiFromConfig());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(5));
    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getAmountOfStockPurchase()).WillOnce(Return(50000));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(12));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(true));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),               false);
    ASSERT_EQ(dialog->ui->makeDecisionTimeoutSpinBox->value(),        5);
    ASSERT_EQ(dialog->ui->useScheduleCheckBox->isChecked(),           false);
    ASSERT_EQ(dialog->ui->scheduleStartTimeEdit->time(),              QTime(11, 15));
    ASSERT_EQ(dialog->ui->scheduleEndTimeEdit->time(),                QTime(20, 40));
    ASSERT_EQ(dialog->ui->limitStockPurchaseCheckBox->isChecked(),    false);
    ASSERT_EQ(dialog->ui->amountOfStockPurchaseSpinBox->value(),      50000);
    ASSERT_EQ(dialog->ui->storageMonthLimitSpinBox->value(),          12);
    ASSERT_EQ(dialog->ui->simulatorConfigCommonCheckBox->isChecked(), false);
    ASSERT_EQ(dialog->ui->autoPilotConfigCommonCheckBox->isChecked(), true);
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

TEST_F(Test_SettingsDialog, Test_on_useScheduleCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->useScheduleCheckBox->blockSignals(true);
    dialog->ui->useScheduleCheckBox->setChecked(false);
    dialog->ui->useScheduleCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setUseSchedule(true));
    dialog->ui->useScheduleCheckBox->setChecked(true);

    // clang-format off
    ASSERT_EQ(dialog->ui->scheduleStartTimeEdit->isEnabled(), true);
    ASSERT_EQ(dialog->ui->scheduleEndTimeEdit->isEnabled(),   true);
    // clang-format on

    EXPECT_CALL(*configMock, setUseSchedule(false));
    dialog->ui->useScheduleCheckBox->setChecked(false);

    // clang-format off
    ASSERT_EQ(dialog->ui->scheduleStartTimeEdit->isEnabled(), false);
    ASSERT_EQ(dialog->ui->scheduleEndTimeEdit->isEnabled(),   false);
    // clang-format on
}

TEST_F(Test_SettingsDialog, Test_on_scheduleStartTimeEdit_timeChanged)
{
    const InSequence seq;

    dialog->ui->scheduleStartTimeEdit->blockSignals(true);
    dialog->ui->scheduleEndTimeEdit->blockSignals(true);
    dialog->ui->scheduleStartTimeEdit->setTime(QTime(10, 15));
    dialog->ui->scheduleEndTimeEdit->setTime(QTime(12, 10));
    dialog->ui->scheduleStartTimeEdit->blockSignals(false);
    dialog->ui->scheduleEndTimeEdit->blockSignals(false);

    EXPECT_CALL(*configMock, setScheduleStartHour(11));
    EXPECT_CALL(*configMock, setScheduleStartMinute(15));
    dialog->ui->scheduleStartTimeEdit->setTime(QTime(11, 15));

    EXPECT_CALL(*configMock, setScheduleStartHour(10));
    EXPECT_CALL(*configMock, setScheduleStartMinute(20));
    dialog->ui->scheduleStartTimeEdit->setTime(QTime(10, 20));

    EXPECT_CALL(*configMock, setScheduleEndHour(12));
    EXPECT_CALL(*configMock, setScheduleEndMinute(30));
    EXPECT_CALL(*configMock, setScheduleStartHour(12));
    EXPECT_CALL(*configMock, setScheduleStartMinute(30));
    dialog->ui->scheduleStartTimeEdit->setTime(QTime(12, 30));

    ASSERT_EQ(dialog->ui->scheduleEndTimeEdit->time(), QTime(12, 30));
}

TEST_F(Test_SettingsDialog, Test_on_scheduleEndTimeEdit_timeChanged)
{
    const InSequence seq;

    dialog->ui->scheduleStartTimeEdit->blockSignals(true);
    dialog->ui->scheduleEndTimeEdit->blockSignals(true);
    dialog->ui->scheduleStartTimeEdit->setTime(QTime(10, 15));
    dialog->ui->scheduleEndTimeEdit->setTime(QTime(12, 10));
    dialog->ui->scheduleStartTimeEdit->blockSignals(false);
    dialog->ui->scheduleEndTimeEdit->blockSignals(false);

    EXPECT_CALL(*configMock, setScheduleEndHour(11));
    EXPECT_CALL(*configMock, setScheduleEndMinute(15));
    dialog->ui->scheduleEndTimeEdit->setTime(QTime(11, 15));

    EXPECT_CALL(*configMock, setScheduleEndHour(12));
    EXPECT_CALL(*configMock, setScheduleEndMinute(20));
    dialog->ui->scheduleEndTimeEdit->setTime(QTime(12, 20));

    EXPECT_CALL(*configMock, setScheduleStartHour(10));
    EXPECT_CALL(*configMock, setScheduleStartMinute(0));
    EXPECT_CALL(*configMock, setScheduleEndHour(10));
    EXPECT_CALL(*configMock, setScheduleEndMinute(0));
    dialog->ui->scheduleEndTimeEdit->setTime(QTime(10, 0));

    ASSERT_EQ(dialog->ui->scheduleStartTimeEdit->time(), QTime(10, 0));
}

TEST_F(Test_SettingsDialog, Test_on_limitStockPurchaseCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->limitStockPurchaseCheckBox->blockSignals(true);
    dialog->ui->limitStockPurchaseCheckBox->setChecked(false);
    dialog->ui->limitStockPurchaseCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setLimitStockPurchase(true));
    dialog->ui->limitStockPurchaseCheckBox->setChecked(true);
    ASSERT_EQ(dialog->ui->amountOfStockPurchaseSpinBox->isEnabled(), true);

    EXPECT_CALL(*configMock, setLimitStockPurchase(false));
    dialog->ui->limitStockPurchaseCheckBox->setChecked(false);
    ASSERT_EQ(dialog->ui->amountOfStockPurchaseSpinBox->isEnabled(), false);
}

TEST_F(Test_SettingsDialog, Test_on_amountOfStockPurchaseSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->amountOfStockPurchaseSpinBox->blockSignals(true);
    dialog->ui->amountOfStockPurchaseSpinBox->setValue(1);
    dialog->ui->amountOfStockPurchaseSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setAmountOfStockPurchase(2));
    dialog->ui->amountOfStockPurchaseSpinBox->setValue(2);

    EXPECT_CALL(*configMock, setAmountOfStockPurchase(3));
    dialog->ui->amountOfStockPurchaseSpinBox->setValue(3);
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

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(true));
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Decision maker");

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(false));
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(false);

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(2), "Auto-pilot");
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

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(true));
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(true);
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Decision maker");
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

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(true));
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Decision maker");

    EXPECT_CALL(*configMock, setAutoPilotConfigCommon(false));
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(false);

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(2), "Auto-pilot");
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

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 3);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Simulation");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(2), "Auto-pilot");

    EXPECT_CALL(*configMock, setSimulatorConfigCommon(true));
    dialog->ui->simulatorConfigCommonCheckBox->setChecked(true);
    dialog->ui->autoPilotConfigCommonCheckBox->setChecked(true);

    ASSERT_EQ(dialog->ui->mainTabWidget->count(), 2);
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(0), "General");
    ASSERT_EQ(dialog->ui->mainTabWidget->tabText(1), "Decision maker");
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
    dialog->ui->makeDecisionTimeoutSpinBox->blockSignals(true);
    dialog->ui->useScheduleCheckBox->blockSignals(true);
    dialog->ui->scheduleStartTimeEdit->blockSignals(true);
    dialog->ui->scheduleEndTimeEdit->blockSignals(true);
    dialog->ui->limitStockPurchaseCheckBox->blockSignals(true);
    dialog->ui->amountOfStockPurchaseSpinBox->blockSignals(true);
    dialog->ui->storageMonthLimitSpinBox->blockSignals(true);
    dialog->ui->simulatorConfigCommonCheckBox->blockSignals(true);
    dialog->ui->autoPilotConfigCommonCheckBox->blockSignals(true);

    EXPECT_CALL(*configMock, makeDefault());

    EXPECT_CALL(*configMock, getScheduleStartHour()).WillOnce(Return(10));
    EXPECT_CALL(*configMock, getScheduleStartMinute()).WillOnce(Return(30));
    EXPECT_CALL(*configMock, getScheduleEndHour()).WillOnce(Return(19));
    EXPECT_CALL(*configMock, getScheduleEndMinute()).WillOnce(Return(15));

    EXPECT_CALL(*simulatorConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*autoPilotConfigWidgetMock, updateUiFromConfig());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(2));
    EXPECT_CALL(*configMock, isUseSchedule()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getAmountOfStockPurchase()).WillOnce(Return(20000));
    EXPECT_CALL(*configMock, getStorageMonthLimit()).WillOnce(Return(36));
    EXPECT_CALL(*configMock, isSimulatorConfigCommon()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isAutoPilotConfigCommon()).WillOnce(Return(false));

    dialog->ui->defaultButton->click();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),               true);
    ASSERT_EQ(dialog->ui->makeDecisionTimeoutSpinBox->value(),        2);
    ASSERT_EQ(dialog->ui->useScheduleCheckBox->isChecked(),           true);
    ASSERT_EQ(dialog->ui->scheduleStartTimeEdit->time(),              QTime(10, 30));
    ASSERT_EQ(dialog->ui->scheduleEndTimeEdit->time(),                QTime(19, 15));
    ASSERT_EQ(dialog->ui->limitStockPurchaseCheckBox->isChecked(),    true);
    ASSERT_EQ(dialog->ui->amountOfStockPurchaseSpinBox->value(),      20000);
    ASSERT_EQ(dialog->ui->storageMonthLimitSpinBox->value(),          36);
    ASSERT_EQ(dialog->ui->simulatorConfigCommonCheckBox->isChecked(), true);
    ASSERT_EQ(dialog->ui->autoPilotConfigCommonCheckBox->isChecked(), false);
    // clang-format on
}
