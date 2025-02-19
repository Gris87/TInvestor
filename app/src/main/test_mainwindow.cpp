#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/private/qcoreapplication_p.h>
#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"
#include "src/dialogs/authdialog/iauthdialog_mock.h"
#include "src/dialogs/authdialog/iauthdialogfactory_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialog_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialogfactory_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/cleanup/icleanupthread_mock.h"
#include "src/threads/makedecision/imakedecisionthread_mock.h"
#include "src/threads/userupdate/iuserupdatethread_mock.h"
#include "src/utils/messagebox/imessagebox_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/trayicon/itrayicon_mock.h"
#include "src/widgets/trayicon/itrayiconfactory_mock.h"



using ::testing::_;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_MainWindow : public ::testing::Test
{
protected:
    void SetUp()
    {
        configMock                           = new StrictMock<ConfigMock>();
        configForSettingsDialogMock          = new StrictMock<ConfigMock>();
        configForSimulationMock              = new StrictMock<ConfigMock>();
        authDialogFactoryMock                = new StrictMock<AuthDialogFactoryMock>();
        settingsDialogFactoryMock            = new StrictMock<SettingsDialogFactoryMock>();
        decisionMakerConfigWidgetFactoryMock = new StrictMock<DecisionMakerConfigWidgetFactoryMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();
        trayIconFactoryMock                  = new StrictMock<TrayIconFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        stocksStorageMock                    = new StrictMock<StocksStorageMock>();
        grpcClientMock                       = new StrictMock<GrpcClientMock>();
        userUpdateThreadMock                 = new StrictMock<UserUpdateThreadMock>();
        cleanupThreadMock                    = new StrictMock<CleanupThreadMock>();
        makeDecisionThreadMock               = new StrictMock<MakeDecisionThreadMock>();
        messageBoxUtilsMock                  = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();
        trayIconMock                         = new StrictMock<TrayIconMock>();

        EXPECT_CALL(*trayIconFactoryMock, newInstance(NotNull())).WillOnce(Return(trayIconMock));

        EXPECT_CALL(*configMock, makeDefault());
        EXPECT_CALL(*configMock, load(settingsEditorMock));
        EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(1));

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/geometry"),    QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/windowState"), QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/pageIndex"),   QVariant(0))).WillOnce(Return(QVariant(0)));
        // clang-format on

        mainWindow = new MainWindow(
            configMock,
            configForSettingsDialogMock,
            configForSimulationMock,
            authDialogFactoryMock,
            settingsDialogFactoryMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            trayIconFactoryMock,
            userStorageMock,
            stocksStorageMock,
            grpcClientMock,
            userUpdateThreadMock,
            cleanupThreadMock,
            makeDecisionThreadMock,
            messageBoxUtilsMock,
            settingsEditorMock
        );
    }

    void TearDown()
    {
        // clang-format off
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/geometry"),    _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/windowState"), _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/pageIndex"),   _));
        // clang-format on

        delete mainWindow;
        delete configMock;
        delete configForSettingsDialogMock;
        delete configForSimulationMock;
        delete authDialogFactoryMock;
        delete settingsDialogFactoryMock;
        delete decisionMakerConfigWidgetFactoryMock;
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
        delete trayIconFactoryMock;
        delete userStorageMock;
        delete stocksStorageMock;
        delete grpcClientMock;
        delete userUpdateThreadMock;
        delete cleanupThreadMock;
        delete makeDecisionThreadMock;
        delete messageBoxUtilsMock;
        delete settingsEditorMock;
        delete trayIconMock;
    }

    MainWindow*                                       mainWindow;
    StrictMock<ConfigMock>*                           configMock;
    StrictMock<ConfigMock>*                           configForSettingsDialogMock;
    StrictMock<ConfigMock>*                           configForSimulationMock;
    StrictMock<AuthDialogFactoryMock>*                authDialogFactoryMock;
    StrictMock<SettingsDialogFactoryMock>*            settingsDialogFactoryMock;
    StrictMock<DecisionMakerConfigWidgetFactoryMock>* decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>*  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>*  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>*  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock>* sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock>* sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock>* sellDecision3ConfigWidgetFactoryMock;
    StrictMock<TrayIconFactoryMock>*                  trayIconFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<StocksStorageMock>*                    stocksStorageMock;
    StrictMock<GrpcClientMock>*                       grpcClientMock;
    StrictMock<UserUpdateThreadMock>*                 userUpdateThreadMock;
    StrictMock<CleanupThreadMock>*                    cleanupThreadMock;
    StrictMock<MakeDecisionThreadMock>*               makeDecisionThreadMock;
    StrictMock<MessageBoxUtilsMock>*                  messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
    StrictMock<TrayIconMock>*                         trayIconMock;
};



TEST_F(Test_MainWindow, Test_constructor_and_destructor)
{
    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->stocksPage);

    // clang-format off
    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(),     true);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(),  false);
    // clang-format on

    // clang-format off
    ASSERT_EQ(mainWindow->userUpdateTimer->interval(),   0);
    ASSERT_EQ(mainWindow->userUpdateTimer->isActive(),   false);
    ASSERT_EQ(mainWindow->cleanupTimer->interval(),      0);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(),      false);
    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer->isActive(), false);
    // clang-format on
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

TEST_F(Test_MainWindow, Test_authFailed)
{
    StrictMock<AuthDialogMock>* authDialogMock = new StrictMock<AuthDialogMock>();

    EXPECT_CALL(*authDialogFactoryMock, newInstance(userStorageMock, messageBoxUtilsMock, NotNull()))
        .WillOnce(Return(authDialogMock));
    EXPECT_CALL(*authDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*authDialogMock, getToken()).WillOnce(Return("CoolToken"));
    EXPECT_CALL(*userStorageMock, setToken(QString("CoolToken")));
    EXPECT_CALL(*userUpdateThreadMock, run());

    mainWindow->authFailed();

    userUpdateThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_userUpdateTimerTicked)
{
    EXPECT_CALL(*userUpdateThreadMock, run());

    mainWindow->userUpdateTimerTicked();

    userUpdateThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_cleanupTimerTicked)
{
    EXPECT_CALL(*cleanupThreadMock, run());

    mainWindow->cleanupTimerTicked();

    cleanupThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_makeDecisionTimerTicked)
{
    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->makeDecisionTimerTicked();

    makeDecisionThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_on_actionAuth_triggered)
{
    ASSERT_EQ(mainWindow->ui->actionAuth->isEnabled(), true);

    EXPECT_CALL(*userUpdateThreadMock, run());

    mainWindow->ui->actionAuth->trigger();

    ASSERT_EQ(mainWindow->ui->actionAuth->isEnabled(), false);

    userUpdateThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_on_actionStocksPage_toggled)
{
    mainWindow->ui->actionStocksPage->toggle();

    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->stocksPage);

    // clang-format off
    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(),     true);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(),  false);
    // clang-format on
}

TEST_F(Test_MainWindow, Test_on_actionSimulationPage_toggled)
{
    mainWindow->ui->actionSimulationPage->toggle();

    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->simulationPage);

    // clang-format off
    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(),     false);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), true);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(),  false);
    // clang-format on
}

TEST_F(Test_MainWindow, Test_on_actionAutoPilotPage_toggled)
{
    mainWindow->ui->actionAutoPilotPage->toggle();

    ASSERT_EQ(mainWindow->ui->stackedWidget->currentWidget(), mainWindow->ui->autoPilotPage);

    // clang-format off
    ASSERT_EQ(mainWindow->ui->actionStocksPage->isChecked(),     false);
    ASSERT_EQ(mainWindow->ui->actionSimulationPage->isChecked(), false);
    ASSERT_EQ(mainWindow->ui->actionAutoPilotPage->isChecked(),  true);
    // clang-format on
}

TEST_F(Test_MainWindow, Test_on_actionSettings_triggered)
{
    StrictMock<SettingsDialogMock>* settingsDialogMock = new StrictMock<SettingsDialogMock>();

    EXPECT_CALL(*configForSettingsDialogMock, assign(configMock));
    EXPECT_CALL(
        *settingsDialogFactoryMock,
        newInstance(
            configForSettingsDialogMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            NotNull()
        )
    )
        .WillOnce(Return(settingsDialogMock));
    EXPECT_CALL(*settingsDialogMock, updateUiFromConfig());

    EXPECT_CALL(*settingsDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*configMock, assign(configForSettingsDialogMock));
    EXPECT_CALL(*configMock, save(settingsEditorMock));

    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(2));

    mainWindow->ui->actionSettings->trigger();

    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 120000);
}

TEST_F(Test_MainWindow, Test_init)
{
    ASSERT_EQ(mainWindow->userUpdateTimer->interval(), 0);
    ASSERT_EQ(mainWindow->userUpdateTimer->isActive(), false);
    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 0);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), false);

    EXPECT_CALL(*userStorageMock, readFromDatabase());
    EXPECT_CALL(*stocksStorageMock, readFromDatabase());
    EXPECT_CALL(*cleanupThreadMock, run());
    EXPECT_CALL(*userUpdateThreadMock, run());

    mainWindow->init();

    ASSERT_EQ(mainWindow->userUpdateTimer->interval(), 15 * 60 * 1000);
    ASSERT_EQ(mainWindow->userUpdateTimer->isActive(), true);
    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 24 * 60 * 60 * 1000);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), true);

    userUpdateThreadMock->wait();
    cleanupThreadMock->wait();
}
