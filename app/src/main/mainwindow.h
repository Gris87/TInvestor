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
#include "src/threads/orderbook/iorderbookthread.h"
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
        IStockTableItemWidgetFactory*      stockTableItemWidgetFactory,
        IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
        IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
        IStocksTableRecordFactory*         stockTableRecordFactory,
        IStocksControlsWidgetFactory*      stocksControlsWidgetFactory,
        IStocksTableWidgetFactory*         stocksTableWidgetFactory,
        IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
        IAccountChartWidgetFactory*        accountChartWidgetFactory,
        ILogsTableWidgetFactory*           logsTableWidgetFactory,
        IPortfolioTableWidgetFactory*      portfolioTableWidgetFactory,
        IDecisionMakerWidgetFactory*       decisionMakerWidgetFactory,
        ITrayIconFactory*                  trayIconFactory,
        IUserStorage*                      userStorage,
        IStocksStorage*                    stocksStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IHttpClient*                       httpClient,
        IGrpcClient*                       grpcClient,
        IUserUpdateThread*                 userUpdateThread,
        IPriceCollectThread*               priceCollectThread,
        ILastPriceThread*                  lastPriceThread,
        ICleanupThread*                    cleanupThread,
        IMakeDecisionThread*               makeDecisionThread,
        IOrderBookThread*                  orderBookThread,
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
    QTimer userUpdateTimer;
    QTimer priceCollectTimer;
    QTimer cleanupTimer;
    QTimer makeDecisionTimer;
    QTimer stocksTableUpdateAllTimer;
    QTimer stocksTableUpdateLastPricesTimer;
    QTimer keepMoneyChangeDelayTimer;

private:
    void updateStackWidgetToolbar() const;
    void startSimulator();
    void stopSimulator();
    void startAutoPilot();
    void stopAutoPilot();
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
    IUserUpdateThread*                 mUserUpdateThread;
    IPriceCollectThread*               mPriceCollectThread;
    ILastPriceThread*                  mLastPriceThread;
    ICleanupThread*                    mCleanupThread;
    IMakeDecisionThread*               mMakeDecisionThread;
    IOrderBookThread*                  mOrderBookThread;
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
    void userUpdateTimerTicked();
    void priceCollectTimerTicked();
    void cleanupTimerTicked();
    void makeDecisionTimerTicked();
    void stocksTableUpdateAllTimerTicked();
    void stocksTableUpdateLastPricesTimerTicked();
    void keepMoneyChangeDelayTimerTicked();
    void notifyInstrumentsProgress(const QString& message) const;
    void stocksChanged();
    void pricesChanged();
    void periodicDataChanged();
    void lastPriceChanged(const QString& uid);
    void dateChangeDateTimeChanged(const QDateTime& dateTime);
    void filterChanged(const Filter& filter);

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
