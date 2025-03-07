#pragma once



#include <QMainWindow>

#include <QCloseEvent>
#include <QTimer>

#include "src/dialogs/authdialog/iauthdialogfactory.h"
#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/cleanup/icleanupthread.h"
#include "src/threads/lastprice/ilastpricethread.h"
#include "src/threads/makedecision/imakedecisionthread.h"
#include "src/threads/pricecollect/ipricecollectthread.h"
#include "src/threads/userupdate/iuserupdatethread.h"
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
        IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        ITrayIconFactory*                  trayIconFactory,
        IUserStorage*                      userStorage,
        IStocksStorage*                    stocksStorage,
        IGrpcClient*                       grpcClient,
        IUserUpdateThread*                 userUpdateThread,
        IPriceCollectThread*               priceCollectThread,
        ILastPriceThread*                  lastPriceThread,
        ICleanupThread*                    cleanupThread,
        IMakeDecisionThread*               makeDecisionThread,
        IMessageBoxUtils*                  messageBoxUtils,
        ISettingsEditor*                   settingsEditor,
        ITableRecordFactory*               tableRecordFactory
    );
    ~MainWindow();

    MainWindow(const MainWindow& another)            = delete;
    MainWindow& operator=(const MainWindow& another) = delete;

    void closeEvent(QCloseEvent* event);
    void init();

    Ui::MainWindow* ui;

    QTimer* userUpdateTimer;
    QTimer* priceCollectTimer;
    QTimer* cleanupTimer;
    QTimer* makeDecisionTimer;
    QTimer* stocksTableUpdateTimer;

private:
    void updateStocksTableWidget();
    void updateStackWidgetToolbar();
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    IConfig*                           mConfig;
    IConfig*                           mConfigForSettingsDialog;
    IConfig*                           mConfigForSimulation;
    IAuthDialogFactory*                mAuthDialogFactory;
    ISettingsDialogFactory*            mSettingsDialogFactory;
    IDecisionMakerConfigWidgetFactory* mDecisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory*  mBuyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory*  mBuyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory*  mBuyDecision3ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory* mSellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory* mSellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory* mSellDecision3ConfigWidgetFactory;
    IUserStorage*                      mUserStorage;
    IStocksStorage*                    mStocksStorage;
    IGrpcClient*                       mGrpcClient;
    IUserUpdateThread*                 mUserUpdateThread;
    IPriceCollectThread*               mPriceCollectThread;
    ILastPriceThread*                  mLastPriceThread;
    ICleanupThread*                    mCleanupThread;
    IMakeDecisionThread*               mMakeDecisionThread;
    IMessageBoxUtils*                  mMessageBoxUtils;
    ISettingsEditor*                   mSettingsEditor;
    ITableRecordFactory*               mTableRecordFactory;
    QMap<QString, ITableRecord*>       mTableRecords;
    QSet<QString>                      mLastPricesUpdates;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
    void authFailed();
    void userUpdateTimerTicked();
    void priceCollectTimerTicked();
    void cleanupTimerTicked();
    void makeDecisionTimerTicked();
    void stocksTableUpdateTimerTicked();
    void notifyStocksProgress(const QString& message);
    void stocksChanged();
    void lastPriceChanged(const QString& uid);

private slots:
    void on_actionAuth_triggered();
    void on_actionStocksPage_toggled(bool checked);
    void on_actionSimulationPage_toggled(bool checked);
    void on_actionAutoPilotPage_toggled(bool checked);
    void on_actionSettings_triggered();
};
