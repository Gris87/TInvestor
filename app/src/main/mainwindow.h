#pragma once



#include <QMainWindow>

#include <QCloseEvent>
#include <QTimer>

#include "src/config/iconfig.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"
#include "src/config/settingsdialog/isettingsdialogfactory.h"
#include "src/config/settingseditor/isettingseditor.h"
#include "src/db/stocks/istocksdatabase.h"
#include "src/widgets/trayicon/itrayiconfactory.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/threads/cleanup/icleanupthread.h"
#include "src/threads/refresh/irefreshthread.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(
        IConfig *config,
        IConfig *configForSettingsDialog,
        ISettingsDialogFactory *settingsDialogFactory,
        IDecisionMakerConfigWidgetFactory *decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        ITrayIconFactory *trayIconFactory,
        ISettingsEditor *settingsEditor,
        IStocksDatabase *stocksDatabase,
        IStocksStorage *stocksStorage,
        ICleanupThread *cleanupThread,
        IRefreshThread *refreshThread
    );
    ~MainWindow();

    MainWindow(const MainWindow &another) = delete;
    MainWindow& operator=(const MainWindow &another) = delete;

    void closeEvent(QCloseEvent *event);
    void init();

    Ui::MainWindow *ui;

    QTimer *cleanupTimer;
    QTimer *refreshTimer;

private:
    void updateStackWidgetToolbar();
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    IConfig                           *mConfig;
    IConfig                           *mConfigForSettingsDialog;
    ISettingsDialogFactory            *mSettingsDialogFactory;
    IDecisionMakerConfigWidgetFactory *mDecisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory  *mBuyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory  *mBuyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory  *mBuyDecision3ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory *mSellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory *mSellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory *mSellDecision3ConfigWidgetFactory;
    ISettingsEditor                   *mSettingsEditor;
    IStocksDatabase                   *mStocksDatabase;
    IStocksStorage                    *mStocksStorage;
    ICleanupThread                    *mCleanupThread;
    IRefreshThread                    *mRefreshThread;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
    void cleanupTimerTicked();
    void refreshTimerTicked();

private slots:
    void on_actionRefreshManually_triggered();
    void on_actionStocksPage_toggled(bool checked);
    void on_actionSimulationPage_toggled(bool checked);
    void on_actionAutoPilotPage_toggled(bool checked);
    void on_actionSettings_triggered();
};
