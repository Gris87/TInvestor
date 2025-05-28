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
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/cleanup/icleanupthread.h"
#include "src/threads/lastprice/ilastpricethread.h"
#include "src/threads/makedecision/imakedecisionthread.h"
#include "src/threads/operations/ioperationsthread.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/threads/portfolio/iportfoliothread.h"
#include "src/threads/pricecollect/ipricecollectthread.h"
#include "src/threads/userupdate/iuserupdatethread.h"
#include "src/utils/autorunenabler/iautorunenabler.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/decisionmakerwidget/idecisionmakerwidgetfactory.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/stockscontrolswidget/istockscontrolswidgetfactory.h"
#include "src/widgets/stockstablewidget/istockstablewidgetfactory.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory.h"
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
        IConfig*                           config,
        IConfig*                           configForSettingsDialog,
        IConfig*                           configForSimulation,
        IAuthDialogFactory*                authDialogFactory,
        ISettingsDialogFactory*            settingsDialogFactory,
        IOrderWavesDialogFactory*          orderWavesDialogFactory,
        IStartSimulationDialogFactory*     startSimulationDialogFactory,
        IStartAutoPilotDialogFactory*      startAutoPilotDialogFactory,
        IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
        IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
        IStocksTableRecordFactory*         stockTableRecordFactory,
        IStocksControlsWidgetFactory*      stocksControlsWidgetFactory,
        IStocksTableWidgetFactory*         stocksTableWidgetFactory,
        IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
        IAccountChartWidgetFactory*        accountChartWidgetFactory,
        ILogsTableWidgetFactory*           logsTableWidgetFactory,
        IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        IPortfolioTreeRecordFactory*       portfolioTreeRecordFactory,
        IDecisionMakerWidgetFactory*       decisionMakerWidgetFactory,
        ITrayIconFactory*                  trayIconFactory,
        IUserStorage*                      userStorage,
        IStocksStorage*                    stocksStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IHttpClient*                       httpClient,
        IGrpcClient*                       grpcClient,
        ICleanupThread*                    cleanupThread,
        IUserUpdateThread*                 userUpdateThread,
        IPriceCollectThread*               priceCollectThread,
        ILastPriceThread*                  lastPriceThread,
        IOperationsThread*                 operationsThread,
        IPortfolioThread*                  portfolioThread,
        IMakeDecisionThread*               makeDecisionThread,
        IOrderBookThread*                  orderBookThread,
        IFileDialogFactory*                fileDialogFactory,
        IMessageBoxUtils*                  messageBoxUtils,
        ISettingsEditor*                   settingsEditor,
        ISettingsEditor*                   simulatorSettingsEditor,
        ISettingsEditor*                   autoPilotSettingsEditor,
        IAutorunEnabler*                   autorunEnabler,
        QWidget*                           parent = nullptr
    );
    ~MainWindow() override;

    MainWindow(const MainWindow& another)            = delete;
    MainWindow& operator=(const MainWindow& another) = delete;

    void closeEvent(QCloseEvent* event) override;
    void init();

    void updateStocksTableWidget();

    Ui::MainWindow* ui;

    bool   authFailedDialogShown;
    QTimer cleanupTimer;
    QTimer userUpdateTimer;
    QTimer priceCollectTimer;
    QTimer makeDecisionTimer;
    QTimer stocksTableUpdateAllTimer;
    QTimer stocksTableUpdateLastPricesTimer;
    QTimer keepMoneyChangeDelayTimer;

private:
    void updateStackWidgetToolbar() const;
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    IConfig*                           mConfig;
    IConfig*                           mConfigForSettingsDialog;
    IConfig*                           mConfigForSimulation;
    IAuthDialogFactory*                mAuthDialogFactory;
    ISettingsDialogFactory*            mSettingsDialogFactory;
    IStartSimulationDialogFactory*     mStartSimulationDialogFactory;
    IStartAutoPilotDialogFactory*      mStartAutoPilotDialogFactory;
    IDecisionMakerConfigWidgetFactory* mDecisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory*  mBuyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory*  mBuyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory*  mBuyDecision3ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory* mSellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory* mSellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory* mSellDecision3ConfigWidgetFactory;
    IStocksControlsWidget*             mStocksControlsWidget;
    IStocksTableWidget*                mStocksTableWidget;
    IDecisionMakerWidget*              mSimulatorDecisionMakerWidget;
    IDecisionMakerWidget*              mAutoPilotDecisionMakerWidget;
    ITrayIcon*                         mTrayIcon;
    IUserStorage*                      mUserStorage;
    IStocksStorage*                    mStocksStorage;
    IInstrumentsStorage*               mInstrumentsStorage;
    IHttpClient*                       mHttpClient;
    IGrpcClient*                       mGrpcClient;
    ICleanupThread*                    mCleanupThread;
    IUserUpdateThread*                 mUserUpdateThread;
    IPriceCollectThread*               mPriceCollectThread;
    ILastPriceThread*                  mLastPriceThread;
    IOperationsThread*                 mOperationsThread;
    IPortfolioThread*                  mPortfolioThread;
    IMakeDecisionThread*               mMakeDecisionThread;
    IOrderBookThread*                  mOrderBookThread;
    IFileDialogFactory*                mFileDialogFactory;
    IMessageBoxUtils*                  mMessageBoxUtils;
    ISettingsEditor*                   mSettingsEditor;
    ISettingsEditor*                   mSimulatorSettingsEditor;
    ISettingsEditor*                   mAutoPilotSettingsEditor;
    IAutorunEnabler*                   mAutorunEnabler;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
    void authFailed(
        grpc::StatusCode   errorCode,
        const QString&     errorCodeString,
        const std::string& errorMessage,
        const std::string& errorDetails
    );
    void cleanupTimerTicked();
    void userUpdateTimerTicked();
    void priceCollectTimerTicked();
    void makeDecisionTimerTicked();
    void stocksTableUpdateAllTimerTicked();
    void stocksTableUpdateLastPricesTimerTicked();
    void keepMoneyChangeDelayTimerTicked();
    void notifyInstrumentsProgress(const QString& message) const;
    void stocksChanged();
    void pricesChanged();
    void periodicDataChanged();
    void lastPriceChanged(const QString& instrumentId);
    void dateChangeDateTimeChanged(const QDateTime& dateTime);
    void filterChanged(const Filter& filter);
    void startSimulator() const;
    void stopSimulator() const;
    void startAutoPilot() const;
    void stopAutoPilot() const;
    void autoPilotOperationsRead(const QList<Operation>& operations);
    void autoPilotOperationsAdded(const QList<Operation>& operations);
    void autoPilotPortfolioChanged(const Portfolio& portfolio);

private slots:
    void on_actionAuth_triggered();
    void on_actionStocksPage_toggled(bool checked);
    void on_actionSimulationPage_toggled(bool checked);
    void on_actionAutoPilotPage_toggled(bool checked);
    void on_actionSettings_triggered();
    void on_startSimulationButton_clicked();
    void on_startAutoPilotButton_clicked();
    void on_keepMoneySpinBox_valueChanged(int value);
};
