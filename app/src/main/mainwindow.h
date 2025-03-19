#pragma once



#include <QMainWindow>

#include <QCloseEvent>
#include <QTimer>

#include "src/dialogs/authdialog/iauthdialogfactory.h"
#include "src/dialogs/marketwavesdialog/imarketwavesdialogfactory.h"
#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/cleanup/icleanupthread.h"
#include "src/threads/lastprice/ilastpricethread.h"
#include "src/threads/makedecision/imakedecisionthread.h"
#include "src/threads/pricecollect/ipricecollectthread.h"
#include "src/threads/userupdate/iuserupdatethread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/filterwidget/filterwidgetfactory.h"
#include "src/widgets/tablerecord/itablerecordfactory.h"
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
        IMarketWavesDialogFactory*         marketWavesDialogFactory,
        IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        IFilterWidgetFactory*              filterWidgetFactory,
        ITrayIconFactory*                  trayIconFactory,
        IUserStorage*                      userStorage,
        IStocksStorage*                    stocksStorage,
        IHttpClient*                       httpClient,
        IGrpcClient*                       grpcClient,
        IUserUpdateThread*                 userUpdateThread,
        IPriceCollectThread*               priceCollectThread,
        ILastPriceThread*                  lastPriceThread,
        ICleanupThread*                    cleanupThread,
        IMakeDecisionThread*               makeDecisionThread,
        IMessageBoxUtils*                  messageBoxUtils,
        ISettingsEditor*                   settingsEditor,
        ISettingsEditor*                   autorunSettingsEditor,
        IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
        ITableRecordFactory*               tableRecordFactory
    );
    ~MainWindow();

    MainWindow(const MainWindow& another)            = delete;
    MainWindow& operator=(const MainWindow& another) = delete;

    void closeEvent(QCloseEvent* event);
    void init();

    void updateStocksTableWidget();

    Ui::MainWindow* ui;

    QTimer*                      authFailedDelayTimer;
    QTimer*                      userUpdateTimer;
    QTimer*                      priceCollectTimer;
    QTimer*                      cleanupTimer;
    QTimer*                      makeDecisionTimer;
    QTimer*                      stocksTableUpdateTimer;
    QMap<QString, ITableRecord*> tableRecords;
    QSet<QString>                lastPricesUpdates;

private:
    void updateStackWidgetToolbar();
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    IConfig*                           mConfig;
    IConfig*                           mConfigForSettingsDialog;
    IConfig*                           mConfigForSimulation;
    IAuthDialogFactory*                mAuthDialogFactory;
    ISettingsDialogFactory*            mSettingsDialogFactory;
    IMarketWavesDialogFactory*         mMarketWavesDialogFactory;
    IDecisionMakerConfigWidgetFactory* mDecisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory*  mBuyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory*  mBuyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory*  mBuyDecision3ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory* mSellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory* mSellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory* mSellDecision3ConfigWidgetFactory;
    IFilterWidget*                     mFilterWidget;
    ITrayIcon*                         mTrayIcon;
    IUserStorage*                      mUserStorage;
    IStocksStorage*                    mStocksStorage;
    IHttpClient*                       mHttpClient;
    IGrpcClient*                       mGrpcClient;
    IUserUpdateThread*                 mUserUpdateThread;
    IPriceCollectThread*               mPriceCollectThread;
    ILastPriceThread*                  mLastPriceThread;
    ICleanupThread*                    mCleanupThread;
    IMakeDecisionThread*               mMakeDecisionThread;
    IMessageBoxUtils*                  mMessageBoxUtils;
    ISettingsEditor*                   mSettingsEditor;
    ISettingsEditor*                   mAutorunSettingsEditor;
    IActionsTableItemWidgetFactory*    mActionsTableItemWidgetFactory;
    ITableRecordFactory*               mTableRecordFactory;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
    void authFailed();
    void authFailedDelayTimerTicked();
    void userUpdateTimerTicked();
    void priceCollectTimerTicked();
    void cleanupTimerTicked();
    void makeDecisionTimerTicked();
    void stocksTableUpdateTimerTicked();
    void notifyStocksProgress(const QString& message);
    void stocksChanged();
    void pricesChanged();
    void paybackChanged();
    void lastPriceChanged(const QString& uid);
    void filterChanged(const Filter& filter);

private slots:
    void on_actionAuth_triggered();
    void on_actionStocksPage_toggled(bool checked);
    void on_actionSimulationPage_toggled(bool checked);
    void on_actionAutoPilotPage_toggled(bool checked);
    void on_actionSettings_triggered();
    void on_dateChangeTimeEdit_dateTimeChanged(const QDateTime& dateTime);
};
