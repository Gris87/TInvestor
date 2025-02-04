#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <gtest/gtest.h>
#include <QtCore/private/qcoreapplication_p.h>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/config/settingsdialog/isettingsdialog_mock.h"
#include "src/config/settingsdialog/isettingsdialogfactory_mock.h"
#include "src/config/settingseditor/isettingseditor_mock.h"
#include "src/db/stocks/istocksdatabase_mock.h"
#include "src/widgets/trayicon/itrayicon_mock.h"
#include "src/widgets/trayicon/itrayiconfactory_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/threads/cleanup/icleanupthread_mock.h"
#include "src/threads/refresh/irefreshthread_mock.h"



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
        configForSettingsDialogMock          = new StrictMock<ConfigMock>();
        settingsDialogFactoryMock            = new StrictMock<SettingsDialogFactoryMock>();
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
        stocksStorageMock                    = new StrictMock<StocksStorageMock>();
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
            configForSettingsDialogMock,
            settingsDialogFactoryMock,
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
            stocksStorageMock,
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
        delete configForSettingsDialogMock;
        delete settingsDialogFactoryMock;
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
        delete stocksStorageMock;
        delete cleanupThreadMock;
        delete refreshThreadMock;
        delete trayIconMock;
    }

    MainWindow                                       *mainWindow;
    StrictMock<ConfigMock>                           *configMock;
    StrictMock<ConfigMock>                           *configForSettingsDialogMock;
    StrictMock<SettingsDialogFactoryMock>            *settingsDialogFactoryMock;
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
    StrictMock<StocksStorageMock>                    *stocksStorageMock;
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

    cleanupThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_refreshTimerTicked)
{
    ASSERT_EQ(mainWindow->refreshTimer->isActive(), false);

    EXPECT_CALL(*refreshThreadMock, process())
        .WillOnce([] { QThread::msleep(200); });

    mainWindow->refreshTimerTicked();
    mainWindow->refreshTimerTicked();

    ASSERT_EQ(mainWindow->refreshTimer->isActive(), true);

    refreshThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_on_actionRefreshManually_triggered)
{
    ASSERT_EQ(mainWindow->refreshTimer->isActive(), false);

    EXPECT_CALL(*refreshThreadMock, process())
        .WillOnce([] { QThread::msleep(200); });

    mainWindow->ui->actionRefreshManually->trigger();
    mainWindow->ui->actionRefreshManually->trigger();

    ASSERT_EQ(mainWindow->refreshTimer->isActive(), true);

    refreshThreadMock->wait();
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

TEST_F(Test_MainWindow, Test_on_actionSettings_triggered)
{
    StrictMock<SettingsDialogMock> *settingsDialogMock = new StrictMock<SettingsDialogMock>();

    EXPECT_CALL(*configForSettingsDialogMock, assign(configMock));
    EXPECT_CALL(*settingsDialogFactoryMock, newInstance(
        configForSettingsDialogMock,
        decisionMakerConfigWidgetFactoryMock,
        buyDecision1ConfigWidgetFactoryMock,
        buyDecision2ConfigWidgetFactoryMock,
        buyDecision3ConfigWidgetFactoryMock,
        sellDecision1ConfigWidgetFactoryMock,
        sellDecision2ConfigWidgetFactoryMock,
        sellDecision3ConfigWidgetFactoryMock,
        NotNull()
    )).WillOnce(Return(settingsDialogMock));
    EXPECT_CALL(*settingsDialogMock, updateUiFromConfig());

    EXPECT_CALL(*settingsDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*configMock, assign(configForSettingsDialogMock));
    EXPECT_CALL(*configMock, save(settingsEditorMock));

    EXPECT_CALL(*configMock, getRefreshTimeout()).WillOnce(Return(2));

    mainWindow->ui->actionSettings->trigger();

    ASSERT_EQ(mainWindow->refreshTimer->interval(), 120000);
}

TEST_F(Test_MainWindow, Test_init)
{
    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 0);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), false);
    ASSERT_EQ(mainWindow->refreshTimer->isActive(), false);

    EXPECT_CALL(*stocksStorageMock, readFromDatabase(stocksDatabaseMock));

    EXPECT_CALL(*cleanupThreadMock, process());
    EXPECT_CALL(*refreshThreadMock, process());

    mainWindow->init();

    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 24 * 60 * 60 * 1000);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), true);
    ASSERT_EQ(mainWindow->refreshTimer->isActive(), true);

    refreshThreadMock->wait();
}
