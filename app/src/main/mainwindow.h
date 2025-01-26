#pragma once



#include <QMainWindow>

#include <QCloseEvent>
#include <QTimer>

#include "src/config/config.h"
#include "src/config/iconfigfactory.h"
#include "src/config/decisions/idecisionmakerconfigfactory.h"
#include "src/db/stocks/stocksdatabase.h"
#include "src/main/trayicon.h"
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
    MainWindow(IConfigFactory *configFactory, IDecisionMakerConfigFactory *decisionMakerConfigFactory, QWidget *parent = nullptr);
    ~MainWindow();

    void init();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void updateStackWidgetToolbar();
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    Ui::MainWindow *ui;

    TrayIcon       *mTrayIcon;
    IConfig        *mConfig;
    IConfig        *mConfigForSettingsDialog;
    QTimer         *mCleanupTimer;
    QTimer         *mRefreshTimer;
    CleanupThread  *mCleanupThread;
    RefreshThread  *mRefreshThread;
    StocksDatabase *mStocksDatabase;
    QList<Stock>   mStocks;

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
