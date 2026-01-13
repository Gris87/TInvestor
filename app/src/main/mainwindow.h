#pragma once



#include <QMainWindow>

#include <QCloseEvent>
#include <QTimer>

#include "src/dialogs/authdialog/iauthdialogfactory.h"
#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"
#include "src/dialogs/startautopilotdialog/istartautopilotdialogfactory.h"
#include "src/dialogs/startsimulationdialog/istartsimulationdialogfactory.h"
#include "src/grpc/igrpcclient.h"
#include "src/grpc/igrpcretryclient.h"
#include "src/storage/bidirinfos/ibidirinfosstorage.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/autopilotdecisionmaker/iautopilotdecisionmakerthread.h"
#include "src/threads/bidirtrading/ibidirtradingthreadfactory.h"
#include "src/threads/bidirtradingcontrol/ibidirtradingcontrolthread.h"
#include "src/threads/cleanup/icleanupthread.h"
#include "src/threads/follow/ifollowthread.h"
#include "src/threads/highliquidity/ihighliquiditythread.h"
#include "src/threads/lastprice/ilastpricethread.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/threads/operations/ioperationsthread.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/threads/portfolio/iportfoliothread.h"
#include "src/threads/portfoliolastprice/iportfoliolastpricethread.h"
#include "src/threads/simulatordaterangedecisionmaker/isimulatordaterangedecisionmakerthread.h"
#include "src/threads/simulatordecisionmaker/isimulatordecisionmakerthread.h"
#include "src/threads/stockcollect/istockcollectthread.h"
#include "src/threads/trading/itradingthreadfactory.h"
#include "src/threads/userupdate/iuserupdatethread.h"
#include "src/utils/autorunenabler/iautorunenabler.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/timeutils/itimeutils.h"
#include "src/utils/tradeutils/itradeutils.h"
#include "src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/stockscontrolswidget/istockscontrolswidgetfactory.h"
#include "src/widgets/stockstablewidget/istockstablewidgetfactory.h"
#include "src/widgets/trayicon/itrayiconfactory.h"



QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(
        IConfig*                                config,
        IConfig*                                configForSettingsDialog,
        IConfig*                                configForSimulation,
        IAuthDialogFactory*                     authDialogFactory,
        ISettingsDialogFactory*                 settingsDialogFactory,
        IOrderWavesDialogFactory*               orderWavesDialogFactory,
        IStartSimulationDialogFactory*          startSimulationDialogFactory,
        IStartAutoPilotDialogFactory*           startAutoPilotDialogFactory,
        IDecisionMakerConfigWidgetFactory*      decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*       buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*       buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*       buyDecision3ConfigWidgetFactory,
        IBuyDecision4ConfigWidgetFactory*       buyDecision4ConfigWidgetFactory,
        IBuyDecision5ConfigWidgetFactory*       buyDecision5ConfigWidgetFactory,
        IBuyDecision6ConfigWidgetFactory*       buyDecision6ConfigWidgetFactory,
        IBuyDecision7ConfigWidgetFactory*       buyDecision7ConfigWidgetFactory,
        IBuyDecision8ConfigWidgetFactory*       buyDecision8ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory*      sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory*      sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory*      sellDecision3ConfigWidgetFactory,
        ISellDecision4ConfigWidgetFactory*      sellDecision4ConfigWidgetFactory,
        IActionsTableItemWidgetFactory*         actionsTableItemWidgetFactory,
        IOrderWavesWidgetFactory*               orderWavesWidgetFactory,
        IStocksControlsWidgetFactory*           stocksControlsWidgetFactory,
        IStocksTableWidgetFactory*              stocksTableWidgetFactory,
        IOperationsTableWidgetFactory*          operationsTableWidgetFactory,
        IAccountChartWidgetFactory*             accountChartWidgetFactory,
        ILogsFilterWidgetFactory*               logsFilterWidgetFactory,
        ILogsTableWidgetFactory*                logsTableWidgetFactory,
        IPortfolioTreeWidgetFactory*            portfolioTreeWidgetFactory,
        IStocksTableModelFactory*               stocksTableModelFactory,
        IOperationsTableModelFactory*           operationsTableModelFactory,
        ILogsTableModelFactory*                 logsTableModelFactory,
        IPortfolioTreeModelFactory*             portfolioTreeModelFactory,
        IDecisionMakerWidgetFactory*            decisionMakerWidgetFactory,
        ITrayIconFactory*                       trayIconFactory,
        IUserStorage*                           userStorage,
        IStocksStorage*                         stocksStorage,
        IInstrumentsStorage*                    instrumentsStorage,
        ILogosStorage*                          logosStorage,
        IBiDirInfosStorage*                     biDirInfosStorage,
        IHttpClient*                            httpClient,
        IGrpcClient*                            grpcClient,
        IGrpcClient*                            grpcClientForOrders,
        IGrpcRetryClient*                       grpcRetryClient,
        ICleanupThread*                         cleanupThread,
        IUserUpdateThread*                      userUpdateThread,
        IStockCollectThread*                    stockCollectThread,
        ILastPriceThread*                       lastPriceThread,
        IPortfolioLastPriceThread*              simulatorPortfolioLastPriceThread,
        IOperationsThread*                      operationsThread,
        ILogsThread*                            logsThread,
        IPortfolioThread*                       portfolioThread,
        IPortfolioLastPriceThread*              autoPilotPortfolioLastPriceThread,
        ISimulatorDecisionMakerThread*          simulatorDecisionMakerThread,
        ISimulatorDateRangeDecisionMakerThread* simulatorDateRangeDecisionMakerThread,
        IAutoPilotDecisionMakerThread*          autoPilotDecisionMakerThread,
        IBiDirTradingControlThread*             biDirTradingControlThread,
        IHighLiquidityThread*                   highLiquidityThread,
        IFollowThread*                          followThread,
        IOrderBookThread*                       orderBookThread,
        ITradingThreadFactory*                  tradingThreadFactory,
        IBiDirTradingThreadFactory*             biDirTradingThreadFactory,
        IFileDialogFactory*                     fileDialogFactory,
        ITimeUtils*                             timeUtils,
        ITradeUtils*                            tradeUtils,
        IMessageBoxUtils*                       messageBoxUtils,
        ISettingsEditor*                        settingsEditor,
        ISettingsEditor*                        simulatorSettingsEditor,
        ISettingsEditor*                        autoPilotSettingsEditor,
        IAutorunEnabler*                        autorunEnabler,
        QWidget*                                parent = nullptr
    );
    ~MainWindow() override;

    MainWindow(const MainWindow& another)            = delete;
    MainWindow& operator=(const MainWindow& another) = delete;

    void closeEvent(QCloseEvent* event) override;
    void init();

    void updateStocksTableWidget();

    Ui::MainWindow* ui;

    QTimer                              cleanupTimer;
    QTimer                              userUpdateTimer;
    QTimer                              stockCollectTimer;
    QTimer                              makeDecisionTimer;
    QTimer                              stocksTableUpdateAllTimer;
    QTimer                              stocksTableUpdateLastPricesTimer;
    QTimer                              simulatorPortfolioUpdateLastPricesTimer;
    QTimer                              autoPilotPortfolioUpdateLastPricesTimer;
    bool                                authFailedDialogShown;
    QMap<QString, ITradingThread*>      tradingThreads;      // Instrument UID => ITradingThread
    QMap<QString, IBiDirTradingThread*> biDirTradingThreads; // Instrument UID => IBiDirTradingThread

private:
    void updateStackWidgetToolbar() const;
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    IConfig*                                mConfig;
    IConfig*                                mConfigForSettingsDialog;
    IConfig*                                mConfigForSimulation;
    IAuthDialogFactory*                     mAuthDialogFactory;
    ISettingsDialogFactory*                 mSettingsDialogFactory;
    IStartSimulationDialogFactory*          mStartSimulationDialogFactory;
    IStartAutoPilotDialogFactory*           mStartAutoPilotDialogFactory;
    IDecisionMakerConfigWidgetFactory*      mDecisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory*       mBuyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory*       mBuyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory*       mBuyDecision3ConfigWidgetFactory;
    IBuyDecision4ConfigWidgetFactory*       mBuyDecision4ConfigWidgetFactory;
    IBuyDecision5ConfigWidgetFactory*       mBuyDecision5ConfigWidgetFactory;
    IBuyDecision6ConfigWidgetFactory*       mBuyDecision6ConfigWidgetFactory;
    IBuyDecision7ConfigWidgetFactory*       mBuyDecision7ConfigWidgetFactory;
    IBuyDecision8ConfigWidgetFactory*       mBuyDecision8ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory*      mSellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory*      mSellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory*      mSellDecision3ConfigWidgetFactory;
    ISellDecision4ConfigWidgetFactory*      mSellDecision4ConfigWidgetFactory;
    IStocksControlsWidget*                  mStocksControlsWidget;
    IStocksTableWidget*                     mStocksTableWidget;
    IDecisionMakerWidget*                   mSimulatorDecisionMakerWidget;
    IDecisionMakerWidget*                   mAutoPilotDecisionMakerWidget;
    ITrayIcon*                              mTrayIcon;
    IUserStorage*                           mUserStorage;
    IStocksStorage*                         mStocksStorage;
    IInstrumentsStorage*                    mInstrumentsStorage;
    ILogosStorage*                          mLogosStorage;
    IBiDirInfosStorage*                     mBiDirInfosStorage;
    IHttpClient*                            mHttpClient;
    IGrpcClient*                            mGrpcClient;
    IGrpcClient*                            mGrpcClientForOrders;
    IGrpcRetryClient*                       mGrpcRetryClient;
    ICleanupThread*                         mCleanupThread;
    IUserUpdateThread*                      mUserUpdateThread;
    IStockCollectThread*                    mStockCollectThread;
    ILastPriceThread*                       mLastPriceThread;
    IPortfolioLastPriceThread*              mSimulatorPortfolioLastPriceThread;
    IOperationsThread*                      mOperationsThread;
    ILogsThread*                            mLogsThread;
    IPortfolioThread*                       mPortfolioThread;
    IPortfolioLastPriceThread*              mAutoPilotPortfolioLastPriceThread;
    ISimulatorDecisionMakerThread*          mSimulatorDecisionMakerThread;
    ISimulatorDateRangeDecisionMakerThread* mSimulatorDateRangeDecisionMakerThread;
    IAutoPilotDecisionMakerThread*          mAutoPilotDecisionMakerThread;
    IBiDirTradingControlThread*             mBiDirTradingControlThread;
    IHighLiquidityThread*                   mHighLiquidityThread;
    IFollowThread*                          mFollowThread;
    IOrderBookThread*                       mOrderBookThread;
    ITradingThreadFactory*                  mTradingThreadFactory;
    IBiDirTradingThreadFactory*             mBiDirTradingThreadFactory;
    ITimeUtils*                             mTimeUtils;
    ITradeUtils*                            mTradeUtils;
    IMessageBoxUtils*                       mMessageBoxUtils;
    ISettingsEditor*                        mSettingsEditor;
    ISettingsEditor*                        mSimulatorSettingsEditor;
    ISettingsEditor*                        mAutoPilotSettingsEditor;
    IAutorunEnabler*                        mAutorunEnabler;
    QString                                 mAutoPilotAccountId;
    QString                                 mAutoPilotAnotherAccountId;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
    void authFailed(const QString& errorCodeString);
    void cleanupTimerTicked();
    void userUpdateTimerTicked();
    void stockCollectTimerTicked();
    void makeDecisionTimerTicked();
    void stocksTableUpdateAllTimerTicked();
    void stocksTableUpdateLastPricesTimerTicked();
    void simulatorPortfolioUpdateLastPricesTimerTicked();
    void autoPilotPortfolioUpdateLastPricesTimerTicked();
    void notifyInstrumentsProgress(const QString& message) const;
    void stocksChanged();
    void pricesChanged();
    void periodicDataChanged();
    void lastPriceChanged(const QString& instrumentId);
    void dateChangeDateTimeChanged(const QDateTime& dateTime);
    void stockFilterChanged(const StockFilter& filter);
    void startSimulator();
    void stopSimulator();
    void startAutoPilot();
    void stopAutoPilot();
    void simulatorStepProgressChanged(int current, int maximum) const;
    void simulatorTotalProgressChanged(int current, int maximum) const;
    void simulatorProgressChanged(int current, int maximum, const QString& remainingTime) const;
    void simulatorResultFound(const QString& result, const QColor& color);
    void simulatorBestResultChanged(const QString& bestResult, const QColor& color) const;
    void simulatorBestConfigFound();
    void simulatorOperationsRead(const QList<Operation>& operations);
    void simulatorOperationsAdded(const QList<Operation>& operations);
    void simulatorLogsRead(const QList<LogEntry>& entries);
    void simulatorLogAdded(const LogEntry& entry);
    void simulatorPortfolioChanged(const Portfolio& portfolio);
    void simulatorPortfolioLastPriceChanged(const QString& instrumentId, float price);
    void autoPilotOperationsRead(const QList<Operation>& operations);
    void autoPilotOperationsAdded(const QList<Operation>& operations);
    void autoPilotLogsRead(const QList<LogEntry>& entries);
    void autoPilotLogAdded(const LogEntry& entry);
    void autoPilotPortfolioChanged(const Portfolio& portfolio);
    void autoPilotPortfolioLastPriceChanged(const QString& instrumentId, float price);
    void autoPilotTradeInstruments(const InstrumentsForTrading& instruments);
    void autoPilotTradingCompleted(const QString& instrumentId);
    void autoPilotBiDirTradeInstruments(const InstrumentsForBiDirTrading& instruments);
    void autoPilotBiDirTradingCompleted(const QString& instrumentId);

private slots:
    void on_actionAuth_triggered();
    void on_actionStocksPage_toggled(bool checked);
    void on_actionSimulationPage_toggled(bool checked);
    void on_actionAutoPilotPage_toggled(bool checked);
    void on_actionSettings_triggered();
    void on_startSimulationButton_clicked();
    void on_startAutoPilotButton_clicked();
};
