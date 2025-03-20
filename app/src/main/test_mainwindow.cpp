#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
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
#include "src/dialogs/marketwavesdialog/imarketwavesdialogfactory_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialog_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialogfactory_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/cleanup/icleanupthread_mock.h"
#include "src/threads/lastprice/ilastpricethread_mock.h"
#include "src/threads/makedecision/imakedecisionthread_mock.h"
#include "src/threads/marketwaves/imarketwavesthread_mock.h"
#include "src/threads/pricecollect/ipricecollectthread_mock.h"
#include "src/threads/userupdate/iuserupdatethread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/messagebox/imessagebox_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/filterwidget/ifilterwidget_mock.h"
#include "src/widgets/filterwidget/ifilterwidgetfactory_mock.h"
#include "src/widgets/tablerecord/itablerecord_mock.h"
#include "src/widgets/tablerecord/itablerecordfactory_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/trayicon/itrayicon_mock.h"
#include "src/widgets/trayicon/itrayiconfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_MainWindow : public ::testing::Test
{
protected:
    void SetUp()
    {
        InSequence seq;

        configMock                           = new StrictMock<ConfigMock>();
        configForSettingsDialogMock          = new StrictMock<ConfigMock>();
        configForSimulationMock              = new StrictMock<ConfigMock>();
        authDialogFactoryMock                = new StrictMock<AuthDialogFactoryMock>();
        settingsDialogFactoryMock            = new StrictMock<SettingsDialogFactoryMock>();
        marketWavesDialogFactoryMock         = new StrictMock<MarketWavesDialogFactoryMock>();
        decisionMakerConfigWidgetFactoryMock = new StrictMock<DecisionMakerConfigWidgetFactoryMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();
        filterWidgetFactoryMock              = new StrictMock<FilterWidgetFactoryMock>();
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
        marketWavesThreadMock                = new StrictMock<MarketWavesThreadMock>();
        messageBoxUtilsMock                  = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();
        autorunSettingsEditorMock            = new StrictMock<SettingsEditorMock>();
        actionsTableItemWidgetFactoryMock    = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        tableRecordFactoryMock               = new StrictMock<TableRecordFactoryMock>();
        filterWidgetMock                     = new StrictMock<FilterWidgetMock>();
        trayIconMock                         = new StrictMock<TrayIconMock>();

        QString appPath = QDir::toNativeSeparators(qApp->applicationFilePath());

        EXPECT_CALL(*filterWidgetFactoryMock, newInstance(NotNull())).WillOnce(Return(filterWidgetMock));
        EXPECT_CALL(*trayIconFactoryMock, newInstance(NotNull())).WillOnce(Return(trayIconMock));

        EXPECT_CALL(*configMock, makeDefault());
        EXPECT_CALL(*configMock, load(settingsEditorMock));
        EXPECT_CALL(*configMock, getMakeDecisionTimeout()).WillOnce(Return(1));
        EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));

        EXPECT_CALL(
            *autorunSettingsEditorMock,
            setValue(QString("CurrentVersion/Run/TInvestor"), QVariant(QString("\"%1\" --autorun").arg(appPath)))
        );

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/geometry"),                     QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/windowState"),                  QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/pageIndex"),                    QVariant(0))).WillOnce(Return(QVariant(0)));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/stocksTableWidget_Stock"),      QVariant(79))).WillOnce(Return(QVariant(100)));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/stocksTableWidget_Price"),      QVariant(61))).WillOnce(Return(QVariant(100)));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/stocksTableWidget_DayChange"),  QVariant(139))).WillOnce(Return(QVariant(100)));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/stocksTableWidget_DateChange"), QVariant(139))).WillOnce(Return(QVariant(100)));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/stocksTableWidget_Payback"),    QVariant(120))).WillOnce(Return(QVariant(100)));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/stocksTableWidget_Actions"),    QVariant(83))).WillOnce(Return(QVariant(100)));
        // clang-format on

        mainWindow = new MainWindow(
            configMock,
            configForSettingsDialogMock,
            configForSimulationMock,
            authDialogFactoryMock,
            settingsDialogFactoryMock,
            marketWavesDialogFactoryMock,
            decisionMakerConfigWidgetFactoryMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            filterWidgetFactoryMock,
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
            marketWavesThreadMock,
            messageBoxUtilsMock,
            settingsEditorMock,
            autorunSettingsEditorMock,
            actionsTableItemWidgetFactoryMock,
            tableRecordFactoryMock
        );
    }

    void TearDown()
    {
        InSequence seq;

        EXPECT_CALL(*lastPriceThreadMock, terminateThread());

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/geometry"),                     _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/windowState"),                  _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/pageIndex"),                    _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/stocksTableWidget_Stock"),      _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/stocksTableWidget_Price"),      _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/stocksTableWidget_DayChange"),  _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/stocksTableWidget_DateChange"), _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/stocksTableWidget_Payback"),    _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/stocksTableWidget_Actions"),    _));
        // clang-format on

        delete mainWindow;
        delete configMock;
        delete configForSettingsDialogMock;
        delete configForSimulationMock;
        delete authDialogFactoryMock;
        delete settingsDialogFactoryMock;
        delete marketWavesDialogFactoryMock;
        delete decisionMakerConfigWidgetFactoryMock;
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
        delete filterWidgetFactoryMock;
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
        delete marketWavesThreadMock;
        delete messageBoxUtilsMock;
        delete settingsEditorMock;
        delete autorunSettingsEditorMock;
        delete actionsTableItemWidgetFactoryMock;
        delete tableRecordFactoryMock;
        // It will be deleted by `delete ui;`
        /*
        delete filterWidgetMock;
        */
        delete trayIconMock;
    }

    MainWindow*                                       mainWindow;
    StrictMock<ConfigMock>*                           configMock;
    StrictMock<ConfigMock>*                           configForSettingsDialogMock;
    StrictMock<ConfigMock>*                           configForSimulationMock;
    StrictMock<AuthDialogFactoryMock>*                authDialogFactoryMock;
    StrictMock<SettingsDialogFactoryMock>*            settingsDialogFactoryMock;
    StrictMock<MarketWavesDialogFactoryMock>*         marketWavesDialogFactoryMock;
    StrictMock<DecisionMakerConfigWidgetFactoryMock>* decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>*  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>*  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>*  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock>* sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock>* sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock>* sellDecision3ConfigWidgetFactoryMock;
    StrictMock<FilterWidgetFactoryMock>*              filterWidgetFactoryMock;
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
    StrictMock<MarketWavesThreadMock>*                marketWavesThreadMock;
    StrictMock<MessageBoxUtilsMock>*                  messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
    StrictMock<SettingsEditorMock>*                   autorunSettingsEditorMock;
    StrictMock<ActionsTableItemWidgetFactoryMock>*    actionsTableItemWidgetFactoryMock;
    StrictMock<TableRecordFactoryMock>*               tableRecordFactoryMock;
    StrictMock<FilterWidgetMock>*                     filterWidgetMock;
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
    // clang-format off
    ASSERT_EQ(mainWindow->authFailedDelayTimer->interval(), 0);
    ASSERT_EQ(mainWindow->authFailedDelayTimer->isActive(), false);
    // clang-format on

    mainWindow->authFailed(grpc::StatusCode::UNKNOWN, "", "");

    // clang-format off
    ASSERT_EQ(mainWindow->authFailedDelayTimer->interval(), 1000);
    ASSERT_EQ(mainWindow->authFailedDelayTimer->isActive(), true);
    // clang-format on
}

TEST_F(Test_MainWindow, Test_authFailedDelayTimerTicked)
{
    // InSequence seq;

    // clang-format off
    ASSERT_EQ(mainWindow->authFailedDelayTimer->interval(), 0);
    ASSERT_EQ(mainWindow->authFailedDelayTimer->isActive(), false);
    // clang-format on

    mainWindow->authFailed(grpc::StatusCode::UNKNOWN, "", "");

    // clang-format off
    ASSERT_EQ(mainWindow->authFailedDelayTimer->interval(), 1000);
    ASSERT_EQ(mainWindow->authFailedDelayTimer->isActive(), true);
    // clang-format on

    StrictMock<AuthDialogMock>* authDialogMock =
        new StrictMock<AuthDialogMock>(); // Will be deleted in authFailedDelayTimerTicked

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

    mainWindow->authFailedDelayTimerTicked();

    // clang-format off
    ASSERT_EQ(mainWindow->authFailedDelayTimer->interval(), 1000);
    ASSERT_EQ(mainWindow->authFailedDelayTimer->isActive(), false);
    // clang-format on

    userUpdateThreadMock->wait();
    priceCollectThreadMock->wait();
    lastPriceThreadMock->wait();
    makeDecisionThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_userUpdateTimerTicked)
{
    InSequence seq;

    EXPECT_CALL(*userUpdateThreadMock, run());

    mainWindow->userUpdateTimerTicked();

    userUpdateThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_priceCollectTimerTicked)
{
    InSequence seq;

    EXPECT_CALL(*priceCollectThreadMock, run());

    mainWindow->priceCollectTimerTicked();

    priceCollectThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_cleanupTimerTicked)
{
    InSequence seq;

    EXPECT_CALL(*cleanupThreadMock, run());

    mainWindow->cleanupTimerTicked();

    cleanupThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_makeDecisionTimerTicked)
{
    InSequence seq;

    EXPECT_CALL(*makeDecisionThreadMock, run());

    mainWindow->makeDecisionTimerTicked();

    makeDecisionThreadMock->wait();
}

TEST_F(Test_MainWindow, Test_stocksTableUpdateTimerTicked)
{
    InSequence seq;

    StrictMock<TableRecordMock> tableRecordMock1;
    StrictMock<TableRecordMock> tableRecordMock2;

    QMutex        mutex;
    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    Filter filter;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock1,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock1));
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock1, updateAll());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock2,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock2));
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->tableRecords.size(),                    2);
    ASSERT_EQ(mainWindow->tableRecords["aaaaa"],                  &tableRecordMock1);
    ASSERT_EQ(mainWindow->tableRecords["bbbbb"],                  &tableRecordMock2);
    // clang-format on

    mainWindow->lastPriceChanged("aaaaa");

    // clang-format off
    ASSERT_EQ(mainWindow->lastPricesUpdates.size(),            1);
    ASSERT_EQ(mainWindow->lastPricesUpdates.contains("aaaaa"), true);
    // clang-format on

    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(tableRecordMock1, updatePrice());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->stocksTableUpdateTimerTicked();

    ASSERT_EQ(mainWindow->lastPricesUpdates.isEmpty(), true);
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
    InSequence seq;

    QMutex        mutex;
    QList<Stock*> stocks;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*lastPriceThreadMock, stocksChanged());

    mainWindow->stocksChanged();
}

TEST_F(Test_MainWindow, Test_pricesChanged)
{
    InSequence seq;

    StrictMock<TableRecordMock> tableRecordMock1;
    StrictMock<TableRecordMock> tableRecordMock2;

    QMutex        mutex;
    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    Filter filter;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock1,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock1));
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock1, updateAll());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock2,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock2));
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->tableRecords.size(),                    2);
    ASSERT_EQ(mainWindow->tableRecords["aaaaa"],                  &tableRecordMock1);
    ASSERT_EQ(mainWindow->tableRecords["bbbbb"],                  &tableRecordMock2);
    // clang-format on

    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(tableRecordMock1, updatePrice());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock2, updatePrice());
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->pricesChanged();
}

TEST_F(Test_MainWindow, Test_paybackChanged)
{
    InSequence seq;

    StrictMock<TableRecordMock> tableRecordMock1;
    StrictMock<TableRecordMock> tableRecordMock2;

    QMutex        mutex;
    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    Filter filter;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock1,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock1));
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock1, updateAll());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock2,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock2));
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->tableRecords.size(),                    2);
    ASSERT_EQ(mainWindow->tableRecords["aaaaa"],                  &tableRecordMock1);
    ASSERT_EQ(mainWindow->tableRecords["bbbbb"],                  &tableRecordMock2);
    // clang-format on

    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(tableRecordMock1, updatePayback());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock2, updatePayback());
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->paybackChanged();
}

TEST_F(Test_MainWindow, Test_lastPriceChanged)
{
    ASSERT_EQ(mainWindow->lastPricesUpdates.isEmpty(), true);

    mainWindow->lastPriceChanged("aaaaa");

    // clang-format off
    ASSERT_EQ(mainWindow->lastPricesUpdates.size(),            1);
    ASSERT_EQ(mainWindow->lastPricesUpdates.contains("aaaaa"), true);
    // clang-format on

    mainWindow->lastPriceChanged("bbbbb");

    // clang-format off
    ASSERT_EQ(mainWindow->lastPricesUpdates.size(),            2);
    ASSERT_EQ(mainWindow->lastPricesUpdates.contains("aaaaa"), true);
    ASSERT_EQ(mainWindow->lastPricesUpdates.contains("bbbbb"), true);
    // clang-format on
}

TEST_F(Test_MainWindow, Test_filterChanged)
{
    InSequence seq;

    StrictMock<TableRecordMock> tableRecordMock1;
    StrictMock<TableRecordMock> tableRecordMock2;

    QMutex        mutex;
    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    Filter filter;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock1,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock1));
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock1, updateAll());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock2,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock2));
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->tableRecords.size(),                    2);
    ASSERT_EQ(mainWindow->tableRecords["aaaaa"],                  &tableRecordMock1);
    ASSERT_EQ(mainWindow->tableRecords["bbbbb"],                  &tableRecordMock2);
    // clang-format on

    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->filterChanged(filter);
}

TEST_F(Test_MainWindow, Test_on_actionAuth_triggered)
{
    // InSequence seq;

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
    InSequence seq;

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

    EXPECT_CALL(*autorunSettingsEditorMock, remove(QString("CurrentVersion/Run/TInvestor")));

    mainWindow->ui->actionSettings->trigger();

    ASSERT_EQ(mainWindow->makeDecisionTimer->interval(), 120000);
}

TEST_F(Test_MainWindow, Test_on_dateChangeTimeEdit_dateTimeChanged)
{
    InSequence seq;

    StrictMock<TableRecordMock> tableRecordMock1;
    StrictMock<TableRecordMock> tableRecordMock2;

    QMutex        mutex;
    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    Filter filter;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock1,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock1));
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock1, updateAll());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock2,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock2));
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->tableRecords.size(),                    2);
    ASSERT_EQ(mainWindow->tableRecords["aaaaa"],                  &tableRecordMock1);
    ASSERT_EQ(mainWindow->tableRecords["bbbbb"],                  &tableRecordMock2);
    // clang-format on

    EXPECT_CALL(*stocksStorageMock, obtainStocksDatePrice(1742106991000));
    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(tableRecordMock1, updatePrice());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock2, updatePrice());
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->ui->dateChangeTimeEdit->setDateTime(QDateTime::fromMSecsSinceEpoch(1742106991000));
}

TEST_F(Test_MainWindow, Test_init)
{
    // InSequence seq;

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
    InSequence seq;

    StrictMock<TableRecordMock> tableRecordMock1;
    StrictMock<TableRecordMock> tableRecordMock2;

    QMutex        mutex;
    QList<Stock*> stocks;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), true);
    // clang-format on

    Stock stock1;
    Stock stock2;

    stock1.meta.uid = "aaaaa";
    stock2.meta.uid = "bbbbb";

    stocks << &stock1 << &stock1 << &stock2;

    Filter filter;

    EXPECT_CALL(*stocksStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(*stocksStorageMock, getStocks()).WillOnce(ReturnRef(stocks));
    EXPECT_CALL(*filterWidgetMock, getFilter()).WillOnce(ReturnRef(filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock1,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock1));
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(tableRecordMock1, updateAll());
    EXPECT_CALL(tableRecordMock1, filter(mainWindow->ui->stocksTableWidget, filter));
    EXPECT_CALL(
        *tableRecordFactoryMock,
        newInstance(
            mainWindow->ui->stocksTableWidget,
            actionsTableItemWidgetFactoryMock,
            marketWavesDialogFactoryMock,
            marketWavesThreadMock,
            httpClientMock,
            &stock2,
            mainWindow
        )
    )
        .WillOnce(Return(&tableRecordMock2));
    EXPECT_CALL(tableRecordMock2, filter(mainWindow->ui->stocksTableWidget, filter));

    mainWindow->updateStocksTableWidget();

    // clang-format off
    ASSERT_EQ(mainWindow->ui->waitingSpinnerWidget->isSpinning(), false);
    ASSERT_EQ(mainWindow->tableRecords.size(),                    2);
    ASSERT_EQ(mainWindow->tableRecords["aaaaa"],                  &tableRecordMock1);
    ASSERT_EQ(mainWindow->tableRecords["bbbbb"],                  &tableRecordMock2);
    // clang-format on
}
