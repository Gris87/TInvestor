#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <gtest/gtest.h>
#include <QtCore/private/qcoreapplication_p.h>

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
#include "src/main/itrayicon_mock.h"
#include "src/main/itrayiconfactory_mock.h"
#include "src/threads/icleanupthread_mock.h"
#include "src/threads/irefreshthread_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::_;
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
        trayIconMock                         = new StrictMock<TrayIconMock>();

        EXPECT_CALL(*trayIconFactoryMock, newInstance(NotNull())).WillOnce(Return(trayIconMock));

        EXPECT_CALL(*configMock, makeDefault());
        EXPECT_CALL(*configMock, load(settingsEditorMock));
        EXPECT_CALL(*configMock, getRefreshTimeout()).WillOnce(Return(1));

        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/geometry"),    QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/windowState"), QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/pageIndex"),   QVariant(0))).WillOnce(Return(QVariant(0)));

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
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/geometry"),    _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/windowState"), _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/pageIndex"),   _));

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
        delete trayIconMock;
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
    StrictMock<TrayIconMock>                         *trayIconMock;
};



TEST_F(Test_MainWindow, Test_constructor_and_destructor)
{
    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->stocksPage);

    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(), true);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(), false);

    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 0);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), false);
    ASSERT_EQ(mainWindow->refreshTimer->interval(), 60000);
    ASSERT_EQ(mainWindow->refreshTimer->isActive(), false);
}

TEST_F(Test_MainWindow, Test_closeEvent)
{
    QCloseEvent event;

    mainWindow->closeEvent(&event);

    QCoreApplicationPrivate::setEventSpontaneous(&event, true);
    mainWindow->show();

    mainWindow->closeEvent(&event);
}

TEST_F(Test_MainWindow, Test_trayIconClicked)
{
    mainWindow->trayIconClicked(QSystemTrayIcon::DoubleClick);

    ASSERT_EQ(mainWindow->isVisible(), true);
}

TEST_F(Test_MainWindow, Test_trayIconShowClicked)
{
    mainWindow->trayIconShowClicked();

    ASSERT_EQ(mainWindow->isVisible(), true);
}

TEST_F(Test_MainWindow, Test_trayIconExitClicked)
{
    mainWindow->trayIconExitClicked();
}

TEST_F(Test_MainWindow, Test_cleanupTimerTicked)
{
    EXPECT_CALL(*refreshThreadMock, process())
    .WillOnce([] { QThread::msleep(200); });
    EXPECT_CALL(*cleanupThreadMock, process());

    mainWindow->refreshTimerTicked();
    mainWindow->cleanupTimerTicked();
}

TEST_F(Test_MainWindow, Test_refreshTimerTicked)
{
    ASSERT_EQ(mainWindow->refreshTimer->isActive(), false);

    EXPECT_CALL(*refreshThreadMock, process())
        .WillOnce([] { QThread::msleep(200); });

    mainWindow->refreshTimerTicked();
    mainWindow->refreshTimerTicked();

    ASSERT_EQ(mainWindow->refreshTimer->isActive(), true);
}

TEST_F(Test_MainWindow, Test_on_actionRefreshManually_triggered)
{
    ASSERT_EQ(mainWindow->refreshTimer->isActive(), false);

    EXPECT_CALL(*refreshThreadMock, process())
        .WillOnce([] { QThread::msleep(200); });

    mainWindow->ui->actionRefreshManually->trigger();
    mainWindow->ui->actionRefreshManually->trigger();

    ASSERT_EQ(mainWindow->refreshTimer->isActive(), true);
}

TEST_F(Test_MainWindow, Test_on_actionStocksPage_toggled)
{
    mainWindow->ui->actionStocksPage->toggle();

    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->stocksPage);

    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(), true);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(), false);
}

TEST_F(Test_MainWindow, Test_on_actionSimulationPage_toggled)
{
    mainWindow->ui->actionSimulationPage->toggle();

    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->simulationPage);

    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), true);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(), false);
}

TEST_F(Test_MainWindow, Test_on_actionAutoPilotPage_toggled)
{
    mainWindow->ui->actionAutoPilotPage->toggle();

    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->autoPilotPage);

    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(), true);
}
