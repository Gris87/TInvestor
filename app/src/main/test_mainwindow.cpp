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
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/cleanup/icleanupthread_mock.h"
#include "src/threads/follow/ifollowthread_mock.h"
#include "src/threads/lastprice/ilastpricethread_mock.h"
#include "src/threads/logs/ilogsthread_mock.h"
#include "src/threads/makedecision/imakedecisionthread_mock.h"
#include "src/threads/operations/ioperationsthread_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/threads/portfolio/iportfoliothread_mock.h"
#include "src/threads/portfoliolastprice/iportfoliolastpricethread_mock.h"
#include "src/threads/pricecollect/ipricecollectthread_mock.h"
#include "src/threads/trading/itradingthread_mock.h"
#include "src/threads/trading/itradingthreadfactory_mock.h"
#include "src/threads/userupdate/iuserupdatethread_mock.h"
#include "src/utils/autorunenabler/iautorunenabler_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/messagebox/imessageboxutils_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/utils/timeutils/itimeutils_mock.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory_mock.h"
#include "src/widgets/actionstableitemwidget/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/decisionmakerwidget/idecisionmakerwidget_mock.h"
#include "src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory_mock.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory_mock.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory_mock.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory_mock.h"
#include "src/widgets/stockscontrolswidget/istockscontrolswidget_mock.h"
#include "src/widgets/stockscontrolswidget/istockscontrolswidgetfactory_mock.h"
#include "src/widgets/stockstablewidget/istockstablewidget_mock.h"
#include "src/widgets/stockstablewidget/istockstablewidgetfactory_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory_mock.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory_mock.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory_mock.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodelfactory_mock.h"
#include "src/widgets/trayicon/itrayicon_mock.h"
#include "src/widgets/trayicon/itrayiconfactory_mock.h"



using ::testing::_;
using ::testing::DoubleEq;
using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



#ifdef Q_OS_WINDOWS
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::DoubleClick;
#else
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::Trigger;
#endif



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
        actionsTableItemWidgetFactoryMock    = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        orderWavesWidgetFactoryMock          = new StrictMock<OrderWavesWidgetFactoryMock>();
        stocksControlsWidgetFactoryMock      = new StrictMock<StocksControlsWidgetFactoryMock>();
        stocksTableWidgetFactoryMock         = new StrictMock<StocksTableWidgetFactoryMock>();
        operationsTableWidgetFactoryMock     = new StrictMock<OperationsTableWidgetFactoryMock>();
        accountChartWidgetFactoryMock        = new StrictMock<AccountChartWidgetFactoryMock>();
        logsFilterWidgetFactoryMock          = new StrictMock<LogsFilterWidgetFactoryMock>();
        logsTableWidgetFactoryMock           = new StrictMock<LogsTableWidgetFactoryMock>();
        portfolioTreeWidgetFactoryMock       = new StrictMock<PortfolioTreeWidgetFactoryMock>();
        stocksTableModelFactoryMock          = new StrictMock<StocksTableModelFactoryMock>();
        operationsTableModelFactoryMock      = new StrictMock<OperationsTableModelFactoryMock>();
        logsTableModelFactoryMock            = new StrictMock<LogsTableModelFactoryMock>();
        portfolioTreeModelFactoryMock        = new StrictMock<PortfolioTreeModelFactoryMock>();
        decisionMakerWidgetFactoryMock       = new StrictMock<DecisionMakerWidgetFactoryMock>();
        trayIconFactoryMock                  = new StrictMock<TrayIconFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        stocksStorageMock                    = new StrictMock<StocksStorageMock>();
        instrumentsStorageMock               = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock                     = new StrictMock<LogosStorageMock>();
        httpClientMock                       = new StrictMock<HttpClientMock>();
        grpcClientMock                       = new StrictMock<GrpcClientMock>();
        cleanupThreadMock                    = new StrictMock<CleanupThreadMock>();
        userUpdateThreadMock                 = new StrictMock<UserUpdateThreadMock>();
        priceCollectThreadMock               = new StrictMock<PriceCollectThreadMock>();
        lastPriceThreadMock                  = new StrictMock<LastPriceThreadMock>();
        operationsThreadMock                 = new StrictMock<OperationsThreadMock>();
        logsThreadMock                       = new StrictMock<LogsThreadMock>();
        portfolioThreadMock                  = new StrictMock<PortfolioThreadMock>();
        portfolioLastPriceThreadMock         = new StrictMock<PortfolioLastPriceThreadMock>();
        followThreadMock                     = new StrictMock<FollowThreadMock>();
        makeDecisionThreadMock               = new StrictMock<MakeDecisionThreadMock>();
        orderBookThreadMock                  = new StrictMock<OrderBookThreadMock>();
        tradingThreadFactoryMock             = new StrictMock<TradingThreadFactoryMock>();
        fileDialogFactoryMock                = new StrictMock<FileDialogFactoryMock>();
        timeUtilsMock                        = new StrictMock<TimeUtilsMock>();
        messageBoxUtilsMock                  = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();
        simulatorSettingsEditorMock          = new StrictMock<SettingsEditorMock>();
        autoPilotSettingsEditorMock          = new StrictMock<SettingsEditorMock>();
        autorunEnablerMock                   = new StrictMock<AutorunEnablerMock>();
        stocksControlsWidgetMock             = new StrictMock<StocksControlsWidgetMock>();
        stocksTableWidgetMock                = new StrictMock<StocksTableWidgetMock>();
        simulatorDecisionMakerWidgetMock     = new StrictMock<DecisionMakerWidgetMock>();
        autoPilotDecisionMakerWidgetMock     = new StrictMock<DecisionMakerWidgetMock>();
        trayIconMock                         = new StrictMock<TrayIconMock>();

        StockFilter filter;

        EXPECT_CALL(*stocksControlsWidgetFactoryMock, newInstance(settingsEditorMock, NotNull()))
            .WillOnce(Return(stocksControlsWidgetMock));
        EXPECT_CALL(
            *stocksTableWidgetFactoryMock,
            newInstance(
                stocksTableModelFactoryMock,
                actionsTableItemWidgetFactoryMock,
                orderWavesDialogFactoryMock,
                orderWavesWidgetFactoryMock,
                userStorageMock,
                orderBookThreadMock,
                httpClientMock,
                fileDialogFactoryMock,
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
                logsFilterWidgetFactoryMock,
                logsTableWidgetFactoryMock,
                portfolioTreeWidgetFactoryMock,
                operationsTableModelFactoryMock,
                logsTableModelFactoryMock,
                portfolioTreeModelFactoryMock,
                fileDialogFactoryMock,
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
                logsFilterWidgetFactoryMock,
                logsTableWidgetFactoryMock,
                portfolioTreeWidgetFactoryMock,
                operationsTableModelFactoryMock,
                logsTableModelFactoryMock,
                portfolioTreeModelFactoryMock,
                fileDialogFactoryMock,
                settingsEditorMock,
                NotNull()
            )
        )
            .WillOnce(Return(autoPilotDecisionMakerWidgetMock));
        EXPECT_CALL(*simulatorDecisionMakerWidgetMock, setAccountName(QString("Simulator")));
        EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, setAccountName(QString("Auto-pilot")));

        EXPECT_CALL(*stocksControlsWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
        EXPECT_CALL(*stocksTableWidgetMock, setFilter(filter));

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

        EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("Options/KeepMoney"), QVariant(0))).WillOnce(Return(QVariant(0)));

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
            actionsTableItemWidgetFactoryMock,
            orderWavesWidgetFactoryMock,
            stocksControlsWidgetFactoryMock,
            stocksTableWidgetFactoryMock,
            operationsTableWidgetFactoryMock,
            accountChartWidgetFactoryMock,
            logsFilterWidgetFactoryMock,
            logsTableWidgetFactoryMock,
            portfolioTreeWidgetFactoryMock,
            stocksTableModelFactoryMock,
            operationsTableModelFactoryMock,
            logsTableModelFactoryMock,
            portfolioTreeModelFactoryMock,
            decisionMakerWidgetFactoryMock,
            trayIconFactoryMock,
            userStorageMock,
            stocksStorageMock,
            instrumentsStorageMock,
            logosStorageMock,
            httpClientMock,
            grpcClientMock,
            cleanupThreadMock,
            userUpdateThreadMock,
            priceCollectThreadMock,
            lastPriceThreadMock,
            operationsThreadMock,
            logsThreadMock,
            portfolioThreadMock,
            portfolioLastPriceThreadMock,
            followThreadMock,
            makeDecisionThreadMock,
            orderBookThreadMock,
            tradingThreadFactoryMock,
            fileDialogFactoryMock,
            timeUtilsMock,
            messageBoxUtilsMock,
            settingsEditorMock,
            simulatorSettingsEditorMock,
            autoPilotSettingsEditorMock,
            autorunEnablerMock
        );
    }

    void TearDown() override
    {
        const InSequence seq;

        EXPECT_CALL(*cleanupThreadMock, terminateThread());
        EXPECT_CALL(*userUpdateThreadMock, terminateThread());
        EXPECT_CALL(*priceCollectThreadMock, terminateThread());
        EXPECT_CALL(*lastPriceThreadMock, terminateThread());
        EXPECT_CALL(*operationsThreadMock, terminateThread());
        EXPECT_CALL(*logsThreadMock, terminateThread());
        EXPECT_CALL(*portfolioThreadMock, terminateThread());
        EXPECT_CALL(*portfolioLastPriceThreadMock, terminateThread());
        EXPECT_CALL(*followThreadMock, terminateThread());
        EXPECT_CALL(*makeDecisionThreadMock, terminateThread());

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
        delete actionsTableItemWidgetFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete stocksControlsWidgetFactoryMock;
        delete stocksTableWidgetFactoryMock;
        delete operationsTableWidgetFactoryMock;
        delete accountChartWidgetFactoryMock;
        delete logsFilterWidgetFactoryMock;
        delete logsTableWidgetFactoryMock;
        delete portfolioTreeWidgetFactoryMock;
        delete stocksTableModelFactoryMock;
        delete operationsTableModelFactoryMock;
        delete logsTableModelFactoryMock;
        delete portfolioTreeModelFactoryMock;
        delete decisionMakerWidgetFactoryMock;
        delete trayIconFactoryMock;
        delete userStorageMock;
        delete stocksStorageMock;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete httpClientMock;
        delete grpcClientMock;
        delete cleanupThreadMock;
        delete userUpdateThreadMock;
        delete priceCollectThreadMock;
        delete lastPriceThreadMock;
        delete operationsThreadMock;
        delete logsThreadMock;
        delete portfolioThreadMock;
        delete portfolioLastPriceThreadMock;
        delete followThreadMock;
        delete makeDecisionThreadMock;
        delete orderBookThreadMock;
        delete tradingThreadFactoryMock;
        delete fileDialogFactoryMock;
        delete timeUtilsMock;
        delete messageBoxUtilsMock;
        delete settingsEditorMock;
        delete simulatorSettingsEditorMock;
        delete autoPilotSettingsEditorMock;
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
    StrictMock<ActionsTableItemWidgetFactoryMock>*    actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>*          orderWavesWidgetFactoryMock;
    StrictMock<StocksControlsWidgetFactoryMock>*      stocksControlsWidgetFactoryMock;
    StrictMock<StocksTableWidgetFactoryMock>*         stocksTableWidgetFactoryMock;
    StrictMock<OperationsTableWidgetFactoryMock>*     operationsTableWidgetFactoryMock;
    StrictMock<AccountChartWidgetFactoryMock>*        accountChartWidgetFactoryMock;
    StrictMock<LogsFilterWidgetFactoryMock>*          logsFilterWidgetFactoryMock;
    StrictMock<LogsTableWidgetFactoryMock>*           logsTableWidgetFactoryMock;
    StrictMock<PortfolioTreeWidgetFactoryMock>*       portfolioTreeWidgetFactoryMock;
    StrictMock<StocksTableModelFactoryMock>*          stocksTableModelFactoryMock;
    StrictMock<OperationsTableModelFactoryMock>*      operationsTableModelFactoryMock;
    StrictMock<LogsTableModelFactoryMock>*            logsTableModelFactoryMock;
    StrictMock<PortfolioTreeModelFactoryMock>*        portfolioTreeModelFactoryMock;
    StrictMock<DecisionMakerWidgetFactoryMock>*       decisionMakerWidgetFactoryMock;
    StrictMock<TrayIconFactoryMock>*                  trayIconFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<StocksStorageMock>*                    stocksStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<LogosStorageMock>*                     logosStorageMock;
    StrictMock<HttpClientMock>*                       httpClientMock;
    StrictMock<GrpcClientMock>*                       grpcClientMock;
    StrictMock<CleanupThreadMock>*                    cleanupThreadMock;
    StrictMock<UserUpdateThreadMock>*                 userUpdateThreadMock;
    StrictMock<PriceCollectThreadMock>*               priceCollectThreadMock;
    StrictMock<LastPriceThreadMock>*                  lastPriceThreadMock;
    StrictMock<OperationsThreadMock>*                 operationsThreadMock;
    StrictMock<LogsThreadMock>*                       logsThreadMock;
    StrictMock<PortfolioThreadMock>*                  portfolioThreadMock;
    StrictMock<PortfolioLastPriceThreadMock>*         portfolioLastPriceThreadMock;
    StrictMock<FollowThreadMock>*                     followThreadMock;
    StrictMock<MakeDecisionThreadMock>*               makeDecisionThreadMock;
    StrictMock<OrderBookThreadMock>*                  orderBookThreadMock;
    StrictMock<TradingThreadFactoryMock>*             tradingThreadFactoryMock;
    StrictMock<FileDialogFactoryMock>*                fileDialogFactoryMock;
    StrictMock<TimeUtilsMock>*                        timeUtilsMock;
    StrictMock<MessageBoxUtilsMock>*                  messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
    StrictMock<SettingsEditorMock>*                   simulatorSettingsEditorMock;
    StrictMock<SettingsEditorMock>*                   autoPilotSettingsEditorMock;
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
    ASSERT_EQ(mainWindow->cleanupTimer.interval(),      0);
    ASSERT_EQ(mainWindow->cleanupTimer.isActive(),      false);
    ASSERT_EQ(mainWindow->userUpdateTimer.interval(),   0);
    ASSERT_EQ(mainWindow->userUpdateTimer.isActive(),   false);
    ASSERT_EQ(mainWindow->priceCollectTimer.interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer.isActive(), false);
    ASSERT_EQ(mainWindow->makeDecisionTimer.interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer.isActive(), false);
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
    mainWindow->trayIconClicked(DOUBLE_CLICK_REASON);

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

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_ERROR, QString(""), QString("GRPC error happened with code UNKNOWN")));
    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_INFO, QString(""), QString("Auto-pilot stopped")));

    EXPECT_CALL(*userUpdateThreadMock, terminateThread());
    EXPECT_CALL(*priceCollectThreadMock, terminateThread());
    EXPECT_CALL(*lastPriceThreadMock, terminateThread());
    EXPECT_CALL(*operationsThreadMock, terminateThread());
    EXPECT_CALL(*logsThreadMock, terminateThread());
    EXPECT_CALL(*portfolioThreadMock, terminateThread());
    EXPECT_CALL(*portfolioLastPriceThreadMock, terminateThread());
    EXPECT_CALL(*followThreadMock, terminateThread());
    EXPECT_CALL(*makeDecisionThreadMock, terminateThread());
    EXPECT_CALL(*authDialogFactoryMock, newInstance(userStorageMock, messageBoxUtilsMock, mainWindow))
        .WillOnce(Return(std::shared_ptr<IAuthDialog>(authDialogMock)));
    EXPECT_CALL(*authDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*authDialogMock, getToken()).WillOnce(Return("CoolToken"));
    EXPECT_CALL(*userStorageMock, setToken(QString("CoolToken")));
    EXPECT_CALL(*simulatorSettingsEditorMock, value(QString("General/Enabled"), QVariant(false)))
        .WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("General/Enabled"), QVariant(false)))
        .WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*userUpdateThreadMock, run());
    EXPECT_CALL(*priceCollectThreadMock, run());
    EXPECT_CALL(*lastPriceThreadMock, run());
    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "Test_authFailed");

    userUpdateThreadMock->wait();
    priceCollectThreadMock->wait();
    lastPriceThreadMock->wait();
    makeDecisionThreadMock->wait();

    mainWindow->authFailedDialogShown = true;

    mainWindow->authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "Test_authFailed");
}

TEST_F(Test_MainWindow, Test_cleanupTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*cleanupThreadMock, run());

    mainWindow->cleanupTimerTicked();

    cleanupThreadMock->wait();
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

    EXPECT_CALL(*stocksTableWidgetMock, updateAll());

    mainWindow->stocksTableUpdateAllTimerTicked();
}

TEST_F(Test_MainWindow, Test_stocksTableUpdateLastPricesTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableWidgetMock, updateLastPrices());

    mainWindow->stocksTableUpdateLastPricesTimerTicked();
}

TEST_F(Test_MainWindow, Test_keepMoneyChangeDelayTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*autoPilotSettingsEditorMock, setValue(QString("Options/KeepMoney"), QVariant(0)));

    mainWindow->keepMoneyChangeDelayTimerTicked();
}

TEST_F(Test_MainWindow, Test_autoPilotPortfolioUpdateLastPricesTimerTicked)
{
    const InSequence seq;

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, updateLastPrices());

    mainWindow->autoPilotPortfolioUpdateLastPricesTimerTicked();
}

TEST_F(Test_MainWindow, Test_notifyInstrumentsProgress)
{
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->text(), "");

    mainWindow->notifyInstrumentsProgress("Step one");
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->text(), "Step one");

    mainWindow->notifyInstrumentsProgress("Step two");
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->text(), "Step two");
}

TEST_F(Test_MainWindow, Test_stocksChanged)
{
    const InSequence seq;

    QList<Stock*> stocks;

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*lastPriceThreadMock, stocksChanged());

    mainWindow->stocksChanged();
}

TEST_F(Test_MainWindow, Test_pricesChanged)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableWidgetMock, updatePrices());

    mainWindow->pricesChanged();
}

TEST_F(Test_MainWindow, Test_periodicDataChanged)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableWidgetMock, updatePeriodicData());

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

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, obtainStocksDatePrice(1767128385000));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*stocksTableWidgetMock, setDateChangeTooltip(QString("From: 2025-12-30 23:59:45")));
    EXPECT_CALL(*stocksTableWidgetMock, updatePrices());

    mainWindow->dateChangeDateTimeChanged(dateChangeTime);
}

TEST_F(Test_MainWindow, Test_stockFilterChanged)
{
    const InSequence seq;

    const StockFilter filter;

    EXPECT_CALL(*stocksTableWidgetMock, setFilter(filter));

    mainWindow->stockFilterChanged(filter);
}

TEST_F(Test_MainWindow, Test_autoPilotOperationsRead)
{
    const InSequence seq;

    const QList<Operation> operations;

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, operationsRead(operations));

    mainWindow->autoPilotOperationsRead(operations);
}

TEST_F(Test_MainWindow, Test_autoPilotOperationsAdded)
{
    const InSequence seq;

    const QList<Operation> operations;

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, operationsAdded(operations));

    mainWindow->autoPilotOperationsAdded(operations);
}

TEST_F(Test_MainWindow, Test_autoPilotPortfolioChanged)
{
    const InSequence seq;

    const Portfolio portfolio;

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, portfolioChanged(portfolio));
    EXPECT_CALL(*portfolioLastPriceThreadMock, portfolioChanged(portfolio));

    mainWindow->autoPilotPortfolioChanged(portfolio);
}

TEST_F(Test_MainWindow, Test_autoPilotLogsRead)
{
    const InSequence seq;

    const QList<LogEntry> entries;

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, logsRead(entries));

    mainWindow->autoPilotLogsRead(entries);
}

TEST_F(Test_MainWindow, Test_autoPilotLogAdded)
{
    const InSequence seq;

    const LogEntry entry;

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, logAdded(entry));

    mainWindow->autoPilotLogAdded(entry);
}

TEST_F(Test_MainWindow, Test_autoPilotPortfolioLastPriceChanged)
{
    const InSequence seq;

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, lastPriceChanged(QString("aaaaa"), FloatEq(1.5f)));

    mainWindow->autoPilotPortfolioLastPriceChanged("aaaaa", 1.5f);
}

TEST_F(Test_MainWindow, Test_autoPilotTradeInstruments_and_autoPilotTradingCompleted)
{
    // const InSequence seq;

    // Will be deleted in MainWindow destructor
    StrictMock<TradingThreadMock>* tradingThreadMock1 = new StrictMock<TradingThreadMock>();
    StrictMock<TradingThreadMock>* tradingThreadMock2 = new StrictMock<TradingThreadMock>();

    ASSERT_EQ(mainWindow->tradingThreads.size(), 0);

    TradingInfo tradingInfo1;
    TradingInfo tradingInfo2;
    TradingInfo tradingInfo3;

    tradingInfo1.expectedCost = 10000.0;
    tradingInfo1.cause        = "Need to buy";

    tradingInfo2.expectedCost = 15000.0;
    tradingInfo2.cause        = "Need to buy more";

    tradingInfo3.expectedCost = 0.0;
    tradingInfo3.cause        = "Sell ASAP";

    QMap<QString, TradingInfo> instruments;
    QMap<QString, TradingInfo> instruments2;

    instruments["aaaaa"] = tradingInfo1;
    instruments["bbbbb"] = tradingInfo2;

    instruments2["aaaaa"] = tradingInfo3;

    EXPECT_CALL(
        *tradingThreadFactoryMock,
        newInstance(
            instrumentsStorageMock,
            grpcClientMock,
            logsThreadMock,
            timeUtilsMock,
            QString(""),
            QString("aaaaa"),
            DoubleEq(10000.0),
            QString("Need to buy"),
            mainWindow
        )
    )
        .WillOnce(Return(tradingThreadMock1));
    EXPECT_CALL(*tradingThreadMock1, run());
    EXPECT_CALL(
        *tradingThreadFactoryMock,
        newInstance(
            instrumentsStorageMock,
            grpcClientMock,
            logsThreadMock,
            timeUtilsMock,
            QString(""),
            QString("bbbbb"),
            DoubleEq(15000.0),
            QString("Need to buy more"),
            mainWindow
        )
    )
        .WillOnce(Return(tradingThreadMock2));
    EXPECT_CALL(*tradingThreadMock2, run());

    mainWindow->autoPilotTradeInstruments(instruments);

    tradingThreadMock1->wait();
    tradingThreadMock2->wait();

    // clang-format off
    ASSERT_EQ(mainWindow->tradingThreads.size(),   2);
    ASSERT_EQ(mainWindow->tradingThreads["aaaaa"], tradingThreadMock1);
    ASSERT_EQ(mainWindow->tradingThreads["bbbbb"], tradingThreadMock2);
    // clang-format on

    EXPECT_CALL(*tradingThreadMock1, setExpectedCost(DoubleEq(0.0), QString("Sell ASAP")));

    mainWindow->autoPilotTradeInstruments(instruments2);

    // clang-format off
    ASSERT_EQ(mainWindow->tradingThreads.size(),   2);
    ASSERT_EQ(mainWindow->tradingThreads["aaaaa"], tradingThreadMock1);
    ASSERT_EQ(mainWindow->tradingThreads["bbbbb"], tradingThreadMock2);
    // clang-format on

    mainWindow->autoPilotTradingCompleted("aaaaa");

    // clang-format off
    ASSERT_EQ(mainWindow->tradingThreads.size(),   1);
    ASSERT_EQ(mainWindow->tradingThreads["bbbbb"], tradingThreadMock2);
    // clang-format on

    // Need to do manually. tradingThreadMock2 will be deleted in MainWindow destructor
    delete tradingThreadMock1;

    EXPECT_CALL(*tradingThreadMock2, terminateThread());
}

TEST_F(Test_MainWindow, Test_on_actionAuth_triggered)
{
    // const InSequence seq;

    ASSERT_EQ(mainWindow->ui->actionAuth->isEnabled(), true);

    // clang-format off
    ASSERT_EQ(mainWindow->cleanupTimer.interval(),      0);
    ASSERT_EQ(mainWindow->cleanupTimer.isActive(),      false);
    ASSERT_EQ(mainWindow->userUpdateTimer.interval(),   0);
    ASSERT_EQ(mainWindow->userUpdateTimer.isActive(),   false);
    ASSERT_EQ(mainWindow->priceCollectTimer.interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer.isActive(), false);
    ASSERT_EQ(mainWindow->makeDecisionTimer.interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer.isActive(), false);
    // clang-format on

    Accounts accounts;
    Account  account;

    account.index = 0;
    account.id    = "aaaaaa";
    account.name  = "Sergio";

    accounts["AAAAAA"] = account;

    EXPECT_CALL(*simulatorSettingsEditorMock, value(QString("General/Enabled"), QVariant(false)))
        .WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("General/Enabled"), QVariant(false)))
        .WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("Options/Mode"), QVariant("VIEW")))
        .WillOnce(Return(QVariant("VIEW")));
    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("Options/Account"), QVariant("")))
        .WillOnce(Return(QVariant("AAAAAA")));

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getAccounts()).WillOnce(ReturnRef(accounts));
    EXPECT_CALL(*userStorageMock, readUnlock());

    EXPECT_CALL(*operationsThreadMock, setAccountId(QString("AAAAAA"), QString("aaaaaa")));
    EXPECT_CALL(*logsThreadMock, setAccountId(QString("AAAAAA"), QString("aaaaaa")));
    EXPECT_CALL(*portfolioThreadMock, setAccountId(QString("aaaaaa")));

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, setAccountName(QString("Sergio")));
    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, showSpinners());

    EXPECT_CALL(*userUpdateThreadMock, run());
    EXPECT_CALL(*priceCollectThreadMock, run());
    EXPECT_CALL(*lastPriceThreadMock, run());
    EXPECT_CALL(*operationsThreadMock, run());
    EXPECT_CALL(*logsThreadMock, run());
    EXPECT_CALL(*portfolioThreadMock, run());
    EXPECT_CALL(*portfolioLastPriceThreadMock, run());
    EXPECT_CALL(*makeDecisionThreadMock, run());

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_INFO, QString(""), QString("Auto-pilot started")));

    mainWindow->ui->actionAuth->trigger();

    ASSERT_EQ(mainWindow->ui->actionAuth->isEnabled(), false);

    // clang-format off
    ASSERT_EQ(mainWindow->cleanupTimer.interval(),      0);
    ASSERT_EQ(mainWindow->cleanupTimer.isActive(),      false);
    ASSERT_EQ(mainWindow->userUpdateTimer.interval(),   0);
    ASSERT_EQ(mainWindow->userUpdateTimer.isActive(),   true);
    ASSERT_EQ(mainWindow->priceCollectTimer.interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer.isActive(), true);
    ASSERT_EQ(mainWindow->makeDecisionTimer.interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer.isActive(), true);
    // clang-format on

    userUpdateThreadMock->wait();
    priceCollectThreadMock->wait();
    lastPriceThreadMock->wait();
    operationsThreadMock->wait();
    logsThreadMock->wait();
    portfolioThreadMock->wait();
    portfolioLastPriceThreadMock->wait();
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

    ASSERT_EQ(mainWindow->makeDecisionTimer.interval(), 120000);
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
    EXPECT_CALL(*simulatorSettingsEditorMock, setValue(QString("General/Enabled"), QVariant(true)));
    EXPECT_CALL(*startSimulationDialogMock, startMoney()).WillOnce(Return(1000000));
    EXPECT_CALL(*simulatorSettingsEditorMock, setValue(QString("Options/StartMoney"), QVariant(1000000)));
    EXPECT_CALL(*startSimulationDialogMock, mode()).WillOnce(Return("DATERANGE"));
    EXPECT_CALL(*simulatorSettingsEditorMock, setValue(QString("Options/Mode"), QVariant("DATERANGE")));
    EXPECT_CALL(*startSimulationDialogMock, fromDate()).WillOnce(Return(QDate(2024, 1, 1)));
    EXPECT_CALL(*simulatorSettingsEditorMock, setValue(QString("Options/FromDate"), QVariant("2024-01-01")));
    EXPECT_CALL(*startSimulationDialogMock, toDate()).WillOnce(Return(QDate(2025, 1, 1)));
    EXPECT_CALL(*simulatorSettingsEditorMock, setValue(QString("Options/ToDate"), QVariant("2025-01-01")));
    EXPECT_CALL(*startSimulationDialogMock, bestConfig()).WillOnce(Return(true));
    EXPECT_CALL(*simulatorSettingsEditorMock, setValue(QString("Options/BestConfig"), QVariant(true)));

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
    EXPECT_CALL(*simulatorSettingsEditorMock, setValue(QString("General/Enabled"), QVariant(false)));

    mainWindow->ui->startSimulationButton->click();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->simulationActiveWidget->isVisible(),         false);
    ASSERT_EQ(mainWindow->ui->simulationActiveSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->ui->startSimulationButton->text(),               "Start simulation");
    // clang-format on
}

TEST_F(Test_MainWindow, Test_on_startAutoPilotButton_clicked)
{
    // const InSequence seq;

    StrictMock<StartAutoPilotDialogMock>* startAutoPilotDialogMock =
        new StrictMock<StartAutoPilotDialogMock>(); // Will be deleted in on_startAutoPilotButton_clicked

    // Will be deleted in stopAutoPilot function
    StrictMock<TradingThreadMock>* tradingThreadMock = new StrictMock<TradingThreadMock>();

    mainWindow->show();
    mainWindow->ui->stackedWidget->setCurrentWidget(mainWindow->ui->autoPilotPage);

    // clang-format off
    ASSERT_EQ(mainWindow->tradingThreads.size(),                          0);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveWidget->isVisible(),         false);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->ui->startAutoPilotButton->text(),               "Start auto-pilot");
    // clang-format on

    Accounts accounts;
    Account  account1;
    Account  account2;

    account1.index = 0;
    account1.id    = "aaaaaa";
    account1.name  = "Sergio";

    account2.index = 0;
    account2.id    = "bbbbbb";
    account2.name  = "Antonio";

    accounts["AAAAAA"] = account1;
    accounts["BBBBBB"] = account2;

    EXPECT_CALL(
        *startAutoPilotDialogFactoryMock, newInstance(userStorageMock, messageBoxUtilsMock, settingsEditorMock, mainWindow)
    )
        .WillOnce(Return(std::shared_ptr<IStartAutoPilotDialog>(startAutoPilotDialogMock)));
    EXPECT_CALL(*startAutoPilotDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*autoPilotSettingsEditorMock, setValue(QString("General/Enabled"), QVariant(true)));
    EXPECT_CALL(*startAutoPilotDialogMock, account()).WillOnce(Return("AAAAAA"));
    EXPECT_CALL(*autoPilotSettingsEditorMock, setValue(QString("Options/Account"), QVariant("AAAAAA")));
    EXPECT_CALL(*startAutoPilotDialogMock, mode()).WillOnce(Return("FOLLOW"));
    EXPECT_CALL(*autoPilotSettingsEditorMock, setValue(QString("Options/Mode"), QVariant("FOLLOW")));
    EXPECT_CALL(*startAutoPilotDialogMock, anotherAccount()).WillOnce(Return("BBBBBB"));
    EXPECT_CALL(*autoPilotSettingsEditorMock, setValue(QString("Options/AnotherAccount"), QVariant("BBBBBB")));

    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("Options/Mode"), QVariant("VIEW")))
        .WillOnce(Return(QVariant("FOLLOW")));
    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("Options/Account"), QVariant("")))
        .WillOnce(Return(QVariant("AAAAAA")));

    EXPECT_CALL(*userStorageMock, readLock());
    EXPECT_CALL(*userStorageMock, getAccounts()).WillOnce(ReturnRef(accounts));
    EXPECT_CALL(*userStorageMock, readUnlock());
    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("Options/AnotherAccount"), QVariant("")))
        .WillOnce(Return(QVariant("BBBBBB")));

    EXPECT_CALL(*operationsThreadMock, setAccountId(QString("AAAAAA"), QString("aaaaaa")));
    EXPECT_CALL(*logsThreadMock, setAccountId(QString("AAAAAA"), QString("aaaaaa")));
    EXPECT_CALL(*portfolioThreadMock, setAccountId(QString("aaaaaa")));
    EXPECT_CALL(*followThreadMock, setAccounts(QString("aaaaaa"), QString("bbbbbb"), QString("Antonio")));

    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, setAccountName(QString("Sergio")));
    EXPECT_CALL(*autoPilotDecisionMakerWidgetMock, showSpinners());

    EXPECT_CALL(*operationsThreadMock, run());
    EXPECT_CALL(*logsThreadMock, run());
    EXPECT_CALL(*portfolioThreadMock, run());
    EXPECT_CALL(*portfolioLastPriceThreadMock, run());
    EXPECT_CALL(*followThreadMock, run());

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_INFO, QString(""), QString("Auto-pilot started")));

    mainWindow->ui->startAutoPilotButton->click();

    // clang-format off
    ASSERT_EQ(mainWindow->tradingThreads.size(),                          0);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveWidget->isVisible(),         true);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveSpinnerWidget->isSpinning(), true);
    ASSERT_EQ(mainWindow->ui->startAutoPilotButton->text(),               "Stop auto-pilot");
    // clang-format on

    operationsThreadMock->wait();
    logsThreadMock->wait();
    portfolioThreadMock->wait();
    portfolioLastPriceThreadMock->wait();
    followThreadMock->wait();

    QMap<QString, TradingInfo> instruments;
    TradingInfo                tradingInfo;

    tradingInfo.expectedCost = 10000.0;
    tradingInfo.cause        = "Need to buy";

    instruments["aaa-aaa"] = tradingInfo;

    EXPECT_CALL(
        *tradingThreadFactoryMock,
        newInstance(
            instrumentsStorageMock,
            grpcClientMock,
            logsThreadMock,
            timeUtilsMock,
            QString("aaaaaa"),
            QString("aaa-aaa"),
            DoubleEq(10000.0),
            QString("Need to buy"),
            mainWindow
        )
    )
        .WillOnce(Return(tradingThreadMock));
    EXPECT_CALL(*tradingThreadMock, run());

    mainWindow->autoPilotTradeInstruments(instruments);

    tradingThreadMock->wait();

    // clang-format off
    ASSERT_EQ(mainWindow->tradingThreads.size(),     1);
    ASSERT_EQ(mainWindow->tradingThreads["aaa-aaa"], tradingThreadMock);
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
    EXPECT_CALL(*autoPilotSettingsEditorMock, setValue(QString("General/Enabled"), QVariant(false)));

    EXPECT_CALL(*logsThreadMock, addLog(LOG_LEVEL_INFO, QString(""), QString("Auto-pilot stopped")));

    EXPECT_CALL(*operationsThreadMock, terminateThread());
    EXPECT_CALL(*logsThreadMock, terminateThread());
    EXPECT_CALL(*portfolioThreadMock, terminateThread());
    EXPECT_CALL(*portfolioLastPriceThreadMock, terminateThread());
    EXPECT_CALL(*followThreadMock, terminateThread());
    EXPECT_CALL(*tradingThreadMock, terminateThread());

    mainWindow->ui->startAutoPilotButton->click();

    // clang-format off
    ASSERT_EQ(mainWindow->tradingThreads.size(),                          0);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveWidget->isVisible(),         false);
    ASSERT_EQ(mainWindow->ui->autoPilotActiveSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->ui->startAutoPilotButton->text(),               "Start auto-pilot");
    // clang-format on
}

TEST_F(Test_MainWindow, Test_on_keepMoneySpinBox_valueChanged)
{
    // clang-format off
    ASSERT_EQ(mainWindow->keepMoneyChangeDelayTimer.interval(), 0);
    ASSERT_EQ(mainWindow->keepMoneyChangeDelayTimer.isActive(), false);
    // clang-format on

    mainWindow->ui->keepMoneySpinBox->setValue(5000);

    // clang-format off
    ASSERT_EQ(mainWindow->keepMoneyChangeDelayTimer.interval(), 1000);
    ASSERT_EQ(mainWindow->keepMoneyChangeDelayTimer.isActive(), true);
    // clang-format on
}

TEST_F(Test_MainWindow, Test_init)
{
    // const InSequence seq;

    QList<Stock*> stocks;

    // clang-format off
    ASSERT_EQ(mainWindow->cleanupTimer.interval(),      0);
    ASSERT_EQ(mainWindow->cleanupTimer.isActive(),      false);
    ASSERT_EQ(mainWindow->userUpdateTimer.interval(),   0);
    ASSERT_EQ(mainWindow->userUpdateTimer.isActive(),   false);
    ASSERT_EQ(mainWindow->priceCollectTimer.interval(), 0);
    ASSERT_EQ(mainWindow->priceCollectTimer.isActive(), false);
    ASSERT_EQ(mainWindow->makeDecisionTimer.interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer.isActive(), false);
    // clang-format on

    EXPECT_CALL(*userStorageMock, readFromDatabase());
    EXPECT_CALL(*stocksStorageMock, readFromDatabase());
    EXPECT_CALL(*instrumentsStorageMock, readFromDatabase());
    EXPECT_CALL(*logosStorageMock, readFromDatabase());
    EXPECT_CALL(*stocksStorageMock, assignLogos());
    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());
    EXPECT_CALL(*simulatorSettingsEditorMock, value(QString("General/Enabled"), QVariant(false)))
        .WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*autoPilotSettingsEditorMock, value(QString("General/Enabled"), QVariant(false)))
        .WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*cleanupThreadMock, run());
    EXPECT_CALL(*userUpdateThreadMock, run());
    EXPECT_CALL(*priceCollectThreadMock, run());
    EXPECT_CALL(*lastPriceThreadMock, run());
    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->init();

    // clang-format off
    ASSERT_EQ(mainWindow->cleanupTimer.interval(),      24 * 60 * 60 * 1000);
    ASSERT_EQ(mainWindow->cleanupTimer.isActive(),      true);
    ASSERT_EQ(mainWindow->userUpdateTimer.interval(),   15 * 60 * 1000);
    ASSERT_EQ(mainWindow->userUpdateTimer.isActive(),   true);
    ASSERT_EQ(mainWindow->priceCollectTimer.interval(), 1 * 60 * 60 * 1000);
    ASSERT_EQ(mainWindow->priceCollectTimer.isActive(), true);
    ASSERT_EQ(mainWindow->makeDecisionTimer.interval(), 60000);
    ASSERT_EQ(mainWindow->makeDecisionTimer.isActive(), true);
    // clang-format on

    cleanupThreadMock->wait();
    userUpdateThreadMock->wait();
    priceCollectThreadMock->wait();
    lastPriceThreadMock->wait();
    makeDecisionThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_updateStocksTableWidget)
{
    const InSequence seq;

    mainWindow->show();

    QList<Stock*> stocks;

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isVisible(),  true);
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), true);
    // clang-format on

    Stock stock1;
    Stock stock2;

    stock1.meta.instrumentId = "aaaaa";
    stock2.meta.instrumentId = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    const QDateTime dateChangeTime(QDate(2023, 12, 30), QTime(23, 59, 45));

    EXPECT_CALL(*stocksStorageMock, readLock());
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*stocksControlsWidgetMock, getDateChangeTime()).WillOnce(Return(dateChangeTime));
    EXPECT_CALL(*stocksStorageMock, obtainStocksDatePrice(1703969985000));
    EXPECT_CALL(*stocksTableWidgetMock, setDateChangeTooltip(QString("From: 2023-12-30 23:59:45")));
    EXPECT_CALL(*stocksTableWidgetMock, updateTable(stocks));
    EXPECT_CALL(*stocksStorageMock, readUnlock());

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isVisible(),  false);
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, cppcoreguidelines-pro-type-static-cast-downcast, readability-function-cognitive-complexity)
