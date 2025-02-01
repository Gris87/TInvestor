#pragma once



#include <QMainWindow>

#include <QCloseEvent>
#include <QTimer>

#include "src/config/iconfig.h"
#include "src/config/decisions/idecisionmakerconfigwidgetfactory.h"
#include "src/config/isettingseditor.h"
#include "src/db/stocks/stocksdatabase.h"
#include "src/main/itrayiconfactory.h"
#include "src/threads/cleanupthread.h"
#include "src/threads/refreshthread.h"



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
        IConfig *сonfig,
        IConfig *сonfigForSettingsDialog,
        IDecisionMakerConfigWidgetFactory *decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        ITrayIconFactory *trayIconFactory,
        ISettingsEditor *settingsEditor
    );
    ~MainWindow();

    MainWindow(const MainWindow &another) = delete;
    MainWindow& operator=(const MainWindow &another) = delete;

    void init();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void updateStackWidgetToolbar();
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    Ui::MainWindow *ui;

    IConfig                           *mConfig;
    IConfig                           *mConfigForSettingsDialog;
    IDecisionMakerConfigWidgetFactory *mDecisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory  *mBuyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory  *mBuyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory  *mBuyDecision3ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory *mSellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory *mSellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory *mSellDecision3ConfigWidgetFactory;
    ISettingsEditor                   *mSettingsEditor;
    QTimer                            *mCleanupTimer;
    QTimer                            *mRefreshTimer;
    CleanupThread                     *mCleanupThread;
    RefreshThread                     *mRefreshThread;
    StocksDatabase                    *mStocksDatabase;
    QList<Stock>                      mStocks;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();

private slots:
    void on_actionRefreshManually_triggered();
    void on_actionStocksPage_toggled(bool checked);
    void on_actionSimulationPage_toggled(bool checked);
    void on_actionAutoPilotPage_toggled(bool checked);
    void on_actionSettings_triggered();
    void cleanupTimerTicked();
    void refreshTimerTicked();
};
