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
#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialog_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialogfactory_mock.h"
#include "src/dialogs/startautopilotdialog/istartautopilotdialog_mock.h"
#include "src/dialogs/startautopilotdialog/istartautopilotdialogfactory_mock.h"
#include "src/dialogs/startsimulationdialog/istartsimulationdialog_mock.h"
#include "src/dialogs/startsimulationdialog/istartsimulationdialogfactory_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/cleanup/icleanupthread_mock.h"
#include "src/threads/lastprice/ilastpricethread_mock.h"
#include "src/threads/makedecision/imakedecisionthread_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/threads/pricecollect/ipricecollectthread_mock.h"
#include "src/threads/userupdate/iuserupdatethread_mock.h"
#include "src/utils/autorunenabler/iautorunenabler_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/messagebox/imessagebox_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/decisionmakerwidget/idecisionmakerwidget_mock.h"
#include "src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory_mock.h"
#include "src/widgets/stockscontrolswidget/istockscontrolswidget_mock.h"
#include "src/widgets/stockscontrolswidget/istockscontrolswidgetfactory_mock.h"
#include "src/widgets/stockstablewidget/istockstablewidget_mock.h"
#include "src/widgets/stockstablewidget/istockstablewidgetfactory_mock.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord_mock.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory_mock.h"
#include "src/widgets/trayicon/itrayicon_mock.h"
#include "src/widgets/trayicon/itrayiconfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, cppcoreguidelines-pro-type-static-cast-downcast, readability-function-cognitive-complexity)
class Test_MainWindow : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        configMock                           = new StrictMock<ConfigMock>();
        configForSettingsDialogMock          = new StrictMock<ConfigMock>();
        configForSimulationMock              = new StrictMock<ConfigMock>();
        authDialogFactoryMock                = new StrictMock<AuthDialogFactoryMock>();
        settingsDialogFactoryMock            = new StrictMock<SettingsDialogFactoryMock>();
        orderWavesDialogFactoryMock          = new StrictMock<OrderWavesDialogFactoryMock>();
        startSimulationDialogFactoryMock     = new StrictMock<StartSimulationDialogFactoryMock>();
        startAutoPilotDialogFactoryMock      = new StrictMock<StartAutoPilotDialogFactoryMock>();
        decisionMakerConfigWidgetFactoryMock = new StrictMock<DecisionMakerConfigWidgetFactoryMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();
        stockTableItemWidgetFactoryMock      = new StrictMock<StockTableItemWidgetFactoryMock>();
        actionsTableItemWidgetFactoryMock    = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        orderWavesWidgetFactoryMock          = new StrictMock<OrderWavesWidgetFactoryMock>();
        stockTableRecordFactoryMock          = new StrictMock<StocksTableRecordFactoryMock>();
        stocksControlsWidgetFactoryMock      = new StrictMock<StocksControlsWidgetFactoryMock>();
        stocksTableWidgetFactoryMock         = new StrictMock<StocksTableWidgetFactoryMock>();
        operationsTableWidgetFactoryMock     = new StrictMock<OperationsTableWidgetFactoryMock>();
        accountChartWidgetFactoryMock        = new StrictMock<AccountChartWidgetFactoryMock>();
        logsTableWidgetFactoryMock           = new StrictMock<LogsTableWidgetFactoryMock>();
        portfolioTableWidgetFactoryMock      = new StrictMock<PortfolioTableWidgetFactoryMock>();
        decisionMakerWidgetFactoryMock       = new StrictMock<DecisionMakerWidgetFactoryMock>();
        trayIconFactoryMock                  = new StrictMock<TrayIconFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        stocksStorageMock                    = new StrictMock<StocksStorageMock>();
        httpClientMock                       = new StrictMock<HttpClientMock>();
        grpcClientMock                       = new StrictMock<GrpcClientMock>();
        userUpdateThreadMock                 = new StrictMock<UserUpdateThreadMock>();
        priceCollectThreadMock               = new StrictMock<PriceCollectThreadMock>();
        lastPriceThreadMock                  = new StrictMock<LastPriceThreadMock>();
        cleanupThreadMock                    = new StrictMock<CleanupThreadMock>();
        makeDecisionThreadMock               = new StrictMock<MakeDecisionThreadMock>();
        orderBookThreadMock                  = new StrictMock<OrderBookThreadMock>();
        messageBoxUtilsMock                  = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();
        autorunEnablerMock                   = new StrictMock<AutorunEnablerMock>();
        stocksControlsWidgetMock             = new StrictMock<StocksControlsWidgetMock>();
        stocksTableWidgetMock                = new StrictMock<StocksTableWidgetMock>();
        simulatorDecisionMakerWidgetMock     = new StrictMock<DecisionMakerWidgetMock>();
        autoPilotDecisionMakerWidgetMock     = new StrictMock<DecisionMakerWidgetMock>();
        trayIconMock                         = new StrictMock<TrayIconMock>();

        EXPECT_CALL(*stocksControlsWidgetFactoryMock, newInstance(stocksStorageMock, settingsEditorMock, NotNull()))
            .WillOnce(Return(stocksControlsWidgetMock));
        EXPECT_CALL(
            *stocksTableWidgetFactoryMock,
            newInstance(
                stockTableRecordFactoryMock,
                stockTableItemWidgetFactoryMock,
                actionsTableItemWidgetFactoryMock,
                orderWavesDialogFactoryMock,
                orderWavesWidgetFactoryMock,
                userStorageMock,
                orderBookThreadMock,
                httpClientMock,
                settingsEditorMock,
                NotNull()
            )
        )
            .WillOnce(Return(stocksTableWidgetMock));
        EXPECT_CALL(
            *decisionMakerWidgetFactoryMock,
            newInstance(
                operationsTableWidgetFactoryMock,
                accountChartWidgetFactoryMock,
                logsTableWidgetFactoryMock,
                portfolioTableWidgetFactoryMock,
                settingsEditorMock,
                NotNull()
            )
        )
            .WillOnce(Return(simulatorDecisionMakerWidgetMock));
        EXPECT_CALL(
            *decisionMakerWidgetFactoryMock,
            newInstance(
                operationsTableWidgetFactoryMock,
                accountChartWidgetFactoryMock,
                logsTableWidgetFactoryMock,
                portfolioTableWidgetFactoryMock,
                settingsEditorMock,
                NotNull()
            )
        )
            .WillOnce(Return(autoPilotDecisionMakerWidgetMock));
        EXPECT_CALL(*trayIconFactoryMock, newInstance(NotNull())).WillOnce(Return(trayIconMock));

        EXPECT_CALL(*configMock, makeDefault());
        EXPECT_CALL(*configMock, load(settingsEditorMock));
        EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(1));
        EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));

        EXPECT_CALL(*autorunEnablerMock, setEnabled(true));

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/geometry"),    QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/windowState"), QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/pageIndex"),   QVariant(0))).WillOnce(Return(QVariant(0)));
        // clang-format on

        EXPECT_CALL(*stocksControlsWidgetMock, loadWindowState(QString("MainWindow/StocksControlsWidget")));
        EXPECT_CALL(*stocksTableWidgetMock, loadWindowState(QString("MainWindow/StocksTableWidget")));
        EXPECT_CALL(*simulatorDecisionMakerWidgetMock, loadWindowState(QString("MainWindow/Simulator")));
        EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, loadWindowState(QString("MainWindow/AutoPilot")));

        mainWindow = new MainWindow(
            configMock,
            configForSettingsDialogMock,
            configForSimulationMock,
            authDialogFactoryMock,
            settingsDialogFactoryMock,
            orderWavesDialogFactoryMock,
            startSimulationDialogFactoryMock,
            startAutoPilotDialogFactoryMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            stockTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesWidgetFactoryMock,
            stockTableRecordFactoryMock,
            stocksControlsWidgetFactoryMock,
            stocksTableWidgetFactoryMock,
            operationsTableWidgetFactoryMock,
            accountChartWidgetFactoryMock,
            logsTableWidgetFactoryMock,
            portfolioTableWidgetFactoryMock,
            decisionMakerWidgetFactoryMock,
            trayIconFactoryMock,
            userStorageMock,
            stocksStorageMock,
            httpClientMock,
            grpcClientMock,
            userUpdateThreadMock,
            priceCollectThreadMock,
            lastPriceThreadMock,
            cleanupThreadMock,
            makeDecisionThreadMock,
            orderBookThreadMock,
            messageBoxUtilsMock,
            settingsEditorMock,
            autorunEnablerMock
        );
    }

    void TearDown() override
    {
        const InSequence seq;

        EXPECT_CALL(*lastPriceThreadMock, terminateThread());

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/geometry"),    _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/windowState"), _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/pageIndex"),   _));
        // clang-format on

        EXPECT_CALL(*stocksControlsWidgetMock, saveWindowState(QString("MainWindow/StocksControlsWidget")));
        EXPECT_CALL(*stocksTableWidgetMock, saveWindowState(QString("MainWindow/StocksTableWidget")));
        EXPECT_CALL(*simulatorDecisionMakerWidgetMock, saveWindowState(QString("MainWindow/Simulator")));
        EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, saveWindowState(QString("MainWindow/AutoPilot")));

        delete mainWindow;
        delete configMock;
        delete configForSettingsDialogMock;
        delete configForSimulationMock;
        delete authDialogFactoryMock;
        delete settingsDialogFactoryMock;
        delete orderWavesDialogFactoryMock;
        delete startSimulationDialogFactoryMock;
        delete startAutoPilotDialogFactoryMock;
        delete decisionMakerConfigWidgetFactoryMock;
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
        delete stockTableItemWidgetFactoryMock;
        delete actionsTableItemWidgetFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete stockTableRecordFactoryMock;
        delete stocksControlsWidgetFactoryMock;
        delete stocksTableWidgetFactoryMock;
        delete operationsTableWidgetFactoryMock;
        delete accountChartWidgetFactoryMock;
        delete logsTableWidgetFactoryMock;
        delete portfolioTableWidgetFactoryMock;
        delete decisionMakerWidgetFactoryMock;
        delete trayIconFactoryMock;
        delete userStorageMock;
        delete stocksStorageMock;
        delete httpClientMock;
        delete grpcClientMock;
        delete userUpdateThreadMock;
        delete priceCollectThreadMock;
        delete lastPriceThreadMock;
        delete cleanupThreadMock;
        delete makeDecisionThreadMock;
        delete orderBookThreadMock;
        delete messageBoxUtilsMock;
        delete settingsEditorMock;
        delete autorunEnablerMock;
        // It will be deleted by `delete ui;`
        /*
        delete stocksControlsWidgetMock;
        delete stocksTableWidgetMock;
        delete simulatorDecisionMakerWidgetMock;
        delete autoPilotDecisionMakerWidgetMock;
        */
        delete trayIconMock;
    }

    MainWindow*                                       mainWindow;
    StrictMock<ConfigMock>*                           configMock;
    StrictMock<ConfigMock>*                           configForSettingsDialogMock;
    StrictMock<ConfigMock>*                           configForSimulationMock;
    StrictMock<AuthDialogFactoryMock>*                authDialogFactoryMock;
    StrictMock<SettingsDialogFactoryMock>*            settingsDialogFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>*          orderWavesDialogFactoryMock;
    StrictMock<StartSimulationDialogFactoryMock>*     startSimulationDialogFactoryMock;
    StrictMock<StartAutoPilotDialogFactoryMock>*      startAutoPilotDialogFactoryMock;
    StrictMock<DecisionMakerConfigWidgetFactoryMock>* decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>*  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>*  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>*  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock>* sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock>* sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock>* sellDecision3ConfigWidgetFactoryMock;
    StrictMock<StockTableItemWidgetFactoryMock>*      stockTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock>*    actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>*          orderWavesWidgetFactoryMock;
    StrictMock<StocksTableRecordFactoryMock>*         stockTableRecordFactoryMock;
    StrictMock<StocksControlsWidgetFactoryMock>*      stocksControlsWidgetFactoryMock;
    StrictMock<StocksTableWidgetFactoryMock>*         stocksTableWidgetFactoryMock;
    StrictMock<OperationsTableWidgetFactoryMock>*     operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*        accountChartWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*           logsTableWidgetFactoryMock;
    StrictMock<PortfolioTableWidgetFactoryMock>*      portfolioTableWidgetFactoryMock;
    StrictMock<DecisionMakerWidgetFactoryMock>*       decisionMakerWidgetFactoryMock;
    StrictMock<TrayIconFactoryMock>*                  trayIconFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<StocksStorageMock>*                    stocksStorageMock;
    StrictMock<HttpClientMock>*                       httpClientMock;
    StrictMock<GrpcClientMock>*                       grpcClientMock;
    StrictMock<UserUpdateThreadMock>*                 userUpdateThreadMock;
    StrictMock<PriceCollectThreadMock>*               priceCollectThreadMock;
    StrictMock<LastPriceThreadMock>*                  lastPriceThreadMock;
    StrictMock<CleanupThreadMock>*                    cleanupThreadMock;
    StrictMock<MakeDecisionThreadMock>*               makeDecisionThreadMock;
    StrictMock<OrderBookThreadMock>*                  orderBookThreadMock;
    StrictMock<MessageBoxUtilsMock>*                  messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
    StrictMock<AutorunEnablerMock>*                   autorunEnablerMock;
    StrictMock<StocksControlsWidgetMock>*             stocksControlsWidgetMock;
    StrictMock<StocksTableWidgetMock>*                stocksTableWidgetMock;
    StrictMock<DecisionMakerWidgetMock>*              simulatorDecisionMakerWidgetMock;
    StrictMock<DecisionMakerWidgetMock>*              autoPilotDecisionMakerWidgetMock;
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
    ASSERT_EQ(mainWindow->priceCollectTimer->interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer->isActive(), false);
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
    // const InSequence seq;

    StrictMock<AuthDialogMock>* authDialogMock = new StrictMock<AuthDialogMock>(); // Will be deleted in authFailed

    EXPECT_CALL(*lastPriceThreadMock, terminateThread());
    EXPECT_CALL(*authDialogFactoryMock, newInstance(userStorageMock, messageBoxUtilsMock, mainWindow))
        .WillOnce(Return(std::shared_ptr<IAuthDialog>(authDialogMock)));
    EXPECT_CALL(*authDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*authDialogMock, getToken()).WillOnce(Return("CoolToken"));
    EXPECT_CALL(*userStorageMock, setToken(QString("CoolToken")));
    EXPECT_CALL(*userUpdateThreadMock, run());
    EXPECT_CALL(*priceCollectThreadMock, run());
    EXPECT_CALL(*lastPriceThreadMock, run());
    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->authFailed(grpc::StatusCode::UNKNOWN, "", "");

    userUpdateThreadMock->wait();
    priceCollectThreadMock->wait();
    lastPriceThreadMock->wait();
    makeDecisionThreadMock->wait();

    mainWindow->authFailedDialogShown = true;

    mainWindow->authFailed(grpc::StatusCode::UNKNOWN, "", "");
}

TEST_F(Test_MainWindow, Test_userUpdateTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*userUpdateThreadMock, run());

    mainWindow->userUpdateTimerTicked();

    userUpdateThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_priceCollectTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*priceCollectThreadMock, run());

    mainWindow->priceCollectTimerTicked();

    priceCollectThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_cleanupTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*cleanupThreadMock, run());

    mainWindow->cleanupTimerTicked();

    cleanupThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_makeDecisionTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->makeDecisionTimerTicked();

    makeDecisionThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_stocksTableUpdateAllTimerTicked)
{
    const InSequence seq;

    Filter filter;

    EXPECT_CALL(*stocksControlsWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*stocksTableWidgetMock, updateAll(filter));

    mainWindow->stocksTableUpdateAllTimerTicked();
}

TEST_F(Test_MainWindow, Test_stocksTableUpdateLastPricesTimerTicked)
{
    const InSequence seq;

    Filter filter;

    EXPECT_CALL(*stocksControlsWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*stocksTableWidgetMock, updateLastPrices(filter));

    mainWindow->stocksTableUpdateLastPricesTimerTicked();
}

TEST_F(Test_MainWindow, Test_notifyStocksProgress)
{
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->text(), "");

    mainWindow->notifyStocksProgress("Step one");
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->text(), "Step one");

    mainWindow->notifyStocksProgress("Step two");
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->text(), "Step two");
}

TEST_F(Test_MainWindow, Test_stocksChanged)
{
    const InSequence seq;

    QMutex        mutex;
    QList<Stock*> stocks;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*lastPriceThreadMock, stocksChanged());

    mainWindow->stocksChanged();
}

TEST_F(Test_MainWindow, Test_pricesChanged)
{
    const InSequence seq;

    Filter filter;

    EXPECT_CALL(*stocksControlsWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*stocksTableWidgetMock, updatePrices(filter));

    mainWindow->pricesChanged();
}

TEST_F(Test_MainWindow, Test_periodicDataChanged)
{
    const InSequence seq;

    Filter filter;

    EXPECT_CALL(*stocksControlsWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*stocksTableWidgetMock, updatePeriodicData(filter));

    mainWindow->periodicDataChanged();
}

TEST_F(Test_MainWindow, Test_lastPriceChanged)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableWidgetMock, lastPriceChanged(QString("aaaa")));

    mainWindow->lastPriceChanged("aaaa");
}

TEST_F(Test_MainWindow, Test_dateChangeDateTimeChanged)
{
    const InSequence seq;

    const QDateTime dateChangeTime(QDate(2025, 12, 30), QTime(23, 59, 45));
    Filter          filter;

    EXPECT_CALL(*stocksTableWidgetMock, setDateChangeTooltip(QString("From: 2025-12-30 23:59:45")));
    EXPECT_CALL(*stocksControlsWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*stocksTableWidgetMock, updatePrices(filter));

    mainWindow->dateChangeDateTimeChanged(dateChangeTime);
}

TEST_F(Test_MainWindow, Test_filterChanged)
{
    const InSequence seq;

    const Filter filter;

    EXPECT_CALL(*stocksTableWidgetMock, filterChanged(filter));

    mainWindow->filterChanged(filter);
}

TEST_F(Test_MainWindow, Test_on_actionAuth_triggered)
{
    // const InSequence seq;

    ASSERT_EQ(mainWindow->ui->actionAuth->isEnabled(), true);

    ASSERT_EQ(mainWindow->userUpdateTimer->interval(), 0);
    ASSERT_EQ(mainWindow->userUpdateTimer->isActive(), false);
    ASSERT_EQ(mainWindow->priceCollectTimer->interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer->isActive(), false);
    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 0);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), false);
    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer->isActive(), false);

    EXPECT_CALL(*userUpdateThreadMock, run());
    EXPECT_CALL(*priceCollectThreadMock, run());
    EXPECT_CALL(*lastPriceThreadMock, run());
    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->ui->actionAuth->trigger();

    ASSERT_EQ(mainWindow->ui->actionAuth->isEnabled(), false);

    ASSERT_EQ(mainWindow->userUpdateTimer->interval(), 0);
    ASSERT_EQ(mainWindow->userUpdateTimer->isActive(), true);
    ASSERT_EQ(mainWindow->priceCollectTimer->interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer->isActive(), true);
    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 0);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), false);
    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer->isActive(), true);

    userUpdateThreadMock->wait();
    priceCollectThreadMock->wait();
    lastPriceThreadMock->wait();
    makeDecisionThreadMock->wait();
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
    const InSequence seq;

    // Will be deleted in on_actionSettings_triggered
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
            mainWindow
        )
    )
        .WillOnce(Return(std::shared_ptr<ISettingsDialog>(settingsDialogMock)));
    EXPECT_CALL(*settingsDialogMock, updateUiFromConfig());

    EXPECT_CALL(*settingsDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*configMock, assign(configForSettingsDialogMock));
    EXPECT_CALL(*configMock, save(settingsEditorMock));

    EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(2));
    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(false));

    EXPECT_CALL(*autorunEnablerMock, setEnabled(false));

    mainWindow->ui->actionSettings->trigger();

    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 120000);
}

TEST_F(Test_MainWindow, Test_on_startSimulationButton_clicked)
{
    const InSequence seq;

    StrictMock<StartSimulationDialogMock>* startSimulationDialogMock =
        new StrictMock<StartSimulationDialogMock>(); // Will be deleted in on_startSimulationButton_clicked

    mainWindow->show();
    mainWindow->ui->stackedWidget->setCurrentWidget(mainWindow->ui->simulationPage);

    // clang-format off
    ASSERT_EQ(mainWindow->ui->simulationActiveWidget->isVisible(),         false);
    ASSERT_EQ(mainWindow->ui->simulationActiveSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->ui->startSimulationButton->text(),               "Start simulation");
    // clang-format on

    EXPECT_CALL(*startSimulationDialogFactoryMock, newInstance(settingsEditorMock, mainWindow))
        .WillOnce(Return(std::shared_ptr<IStartSimulationDialog>(startSimulationDialogMock)));
    EXPECT_CALL(*startSimulationDialogMock, exec()).WillOnce(Return(QDialog::Accepted));

    mainWindow->ui->startSimulationButton->click();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->simulationActiveWidget->isVisible(),         true);
    ASSERT_EQ(mainWindow->ui->simulationActiveSpinnerWidget->isSpinning(), true);
    ASSERT_EQ(mainWindow->ui->startSimulationButton->text(),               "Stop simulation");
    // clang-format on

    EXPECT_CALL(
        *messageBoxUtilsMock,
        question(
            mainWindow,
            QString("Stop simulation"),
            QString("Do you really want to stop simulation?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::NoButton
        )
    )
        .WillOnce(Return(QMessageBox::Yes));

    mainWindow->ui->startSimulationButton->click();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->simulationActiveWidget->isVisible(),         false);
    ASSERT_EQ(mainWindow->ui->simulationActiveSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->ui->startSimulationButton->text(),               "Start simulation");
    // clang-format on
}

TEST_F(Test_MainWindow, Test_on_startAutoPilotButton_clicked)
{
    const InSequence seq;

    StrictMock<StartAutoPilotDialogMock>* startAutoPilotDialogMock =
        new StrictMock<StartAutoPilotDialogMock>(); // Will be deleted in on_startAutoPilotButton_clicked

    mainWindow->show();
    mainWindow->ui->stackedWidget->setCurrentWidget(mainWindow->ui->autoPilotPage);

    // clang-format off
    ASSERT_EQ(mainWindow->ui->autoPilotActiveWidget->isVisible(),         false);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->ui->startAutoPilotButton->text(),               "Start auto-pilot");
    // clang-format on

    EXPECT_CALL(
        *startAutoPilotDialogFactoryMock, newInstance(userStorageMock, messageBoxUtilsMock, settingsEditorMock, mainWindow)
    )
        .WillOnce(Return(std::shared_ptr<IStartAutoPilotDialog>(startAutoPilotDialogMock)));
    EXPECT_CALL(*startAutoPilotDialogMock, exec()).WillOnce(Return(QDialog::Accepted));

    mainWindow->ui->startAutoPilotButton->click();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->autoPilotActiveWidget->isVisible(),         true);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveSpinnerWidget->isSpinning(), true);
    ASSERT_EQ(mainWindow->ui->startAutoPilotButton->text(),               "Stop auto-pilot");
    // clang-format on

    EXPECT_CALL(
        *messageBoxUtilsMock,
        question(
            mainWindow,
            QString("Stop auto-pilot"),
            QString("Do you really want to stop auto-pilot?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::NoButton
        )
    )
        .WillOnce(Return(QMessageBox::Yes));

    mainWindow->ui->startAutoPilotButton->click();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->autoPilotActiveWidget->isVisible(),         false);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->ui->startAutoPilotButton->text(),               "Start auto-pilot");
    // clang-format on
}

TEST_F(Test_MainWindow, Test_init)
{
    // const InSequence seq;

    QMutex        mutex;
    QList<Stock*> stocks;

    ASSERT_EQ(mainWindow->userUpdateTimer->interval(), 0);
    ASSERT_EQ(mainWindow->userUpdateTimer->isActive(), false);
    ASSERT_EQ(mainWindow->priceCollectTimer->interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer->isActive(), false);
    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 0);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), false);
    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer->isActive(), false);

    EXPECT_CALL(*userStorageMock, readFromDatabase());
    EXPECT_CALL(*stocksStorageMock, readFromDatabase());
    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*cleanupThreadMock, run());
    EXPECT_CALL(*userUpdateThreadMock, run());
    EXPECT_CALL(*priceCollectThreadMock, run());
    EXPECT_CALL(*lastPriceThreadMock, run());
    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->init();

    ASSERT_EQ(mainWindow->userUpdateTimer->interval(), 15 * 60 * 1000);
    ASSERT_EQ(mainWindow->userUpdateTimer->isActive(), true);
    ASSERT_EQ(mainWindow->priceCollectTimer->interval(), 1 * 60 * 60 * 1000);
    ASSERT_EQ(mainWindow->priceCollectTimer->isActive(), true);
    ASSERT_EQ(mainWindow->cleanupTimer->interval(), 24 * 60 * 60 * 1000);
    ASSERT_EQ(mainWindow->cleanupTimer->isActive(), true);
    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer->isActive(), true);

    userUpdateThreadMock->wait();
    priceCollectThreadMock->wait();
    lastPriceThreadMock->wait();
    cleanupThreadMock->wait();
    makeDecisionThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_updateStocksTableWidget)
{
    const InSequence seq;

    mainWindow->show();

    QMutex        mutex;
    QList<Stock*> stocks;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isVisible(),  true);
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), true);
    // clang-format on

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const QDateTime dateChangeTime(QDate(2023, 12, 30), QTime(23, 59, 45));
    Filter          filter;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksControlsWidgetMock, getDateChangeTime()).WillOnce(Return(dateChangeTime));
    EXPECT_CALL(*stocksStorageMock, obtainStocksDatePrice(1703969985000));
    EXPECT_CALL(*stocksTableWidgetMock, setDateChangeTooltip(QString("From: 2023-12-30 23:59:45")));
    EXPECT_CALL(*stocksControlsWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(*stocksTableWidgetMock, updateTable(stocks, filter));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isVisible(),  false);
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, cppcoreguidelines-pro-type-static-cast-downcast, readability-function-cognitive-complexity)
