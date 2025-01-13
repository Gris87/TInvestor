#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QCloseEvent>

#include "src/config/config.h"
#include "src/main/trayicon.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void updateStackWidgetToolbar();
    void saveWindowState();
    void loadWindowState();

    Ui::MainWindow *ui;

    TrayIcon *mTrayIcon;
    Config   mConfig;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();

private slots:
    void on_actionRefreshManually_triggered();
    void on_actionFilterStocks_toggled(bool checked);
    void on_actionStocksPage_toggled(bool checked);
    void on_actionSimulationPage_toggled(bool checked);
    void on_actionAutoPilotPage_toggled(bool checked);
    void on_actionSettings_triggered();
};



#endif // MAINWINDOW_H
