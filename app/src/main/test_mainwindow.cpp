#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/sell/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/iselldecision3configwidgetfactory_mock.h"
#include "src/config/decisions/idecisionmakerconfigwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/config/isettingseditor_mock.h"
#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/main/itrayiconfactory_mock.h"
#include "src/threads/icleanupthread_mock.h"
#include "src/threads/irefreshthread_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



class Test_MainWindow : public ::testing::Test
{
protected:
    void SetUp()
    {
        configMock                           = new StrictMock<ConfigMock>();
        сonfigForSettingsDialogMock          = new StrictMock<ConfigMock>();
        decisionMakerConfigWidgetFactoryMock = new StrictMock<DecisionMakerConfigWidgetFactoryMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();
        trayIconFactoryMock                  = new StrictMock<TrayIconFactoryMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();
        stocksDatabaseMock                   = new StrictMock<StocksDatabaseMock>();
        cleanupThreadMock                    = new StrictMock<CleanupThreadMock>();
        refreshThreadMock                    = new StrictMock<RefreshThreadMock>();

        mainWindow = new MainWindow(
            configMock,
            сonfigForSettingsDialogMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            trayIconFactoryMock,
            settingsEditorMock,
            stocksDatabaseMock,
            cleanupThreadMock,
            refreshThreadMock
        );
    }

    void TearDown()
    {
        delete mainWindow;
        delete configMock;
        delete сonfigForSettingsDialogMock;
        delete decisionMakerConfigWidgetFactoryMock;
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
        delete trayIconFactoryMock;
        delete settingsEditorMock;
        delete stocksDatabaseMock;
        delete cleanupThreadMock;
        delete refreshThreadMock;
    }

    MainWindow                                       *mainWindow;
    StrictMock<ConfigMock>                           *configMock;
    StrictMock<ConfigMock>                           *сonfigForSettingsDialogMock;
    StrictMock<DecisionMakerConfigWidgetFactoryMock> *decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>  *buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>  *buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>  *buyDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock> *sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock> *sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock> *sellDecision3ConfigWidgetFactoryMock;
    StrictMock<TrayIconFactoryMock>                  *trayIconFactoryMock;
    StrictMock<SettingsEditorMock>                   *settingsEditorMock;
    StrictMock<StocksDatabaseMock>                   *stocksDatabaseMock;
    StrictMock<CleanupThreadMock>                    *cleanupThreadMock;
    StrictMock<RefreshThreadMock>                    *refreshThreadMock;
};



TEST_F(Test_MainWindow, Test_constructor_and_destructor)
{
}
