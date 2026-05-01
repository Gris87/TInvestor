#pragma once



#include <QMainWindow>

#include <QCloseEvent>
#include <QTimer>

#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"
#include "src/threads/request/irequestthread.h"
#include "src/utils/autorunenabler/iautorunenabler.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/notificationstablewidget/inotificationstablewidgetfactory.h"
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
        IConfig*                          config,
        IConfig*                          configForSettingsDialog,
        ISettingsDialogFactory*           settingsDialogFactory,
        INotificationsTableWidgetFactory* notificationsTableWidgetFactory,
        INotificationsTableModelFactory*  notificationsTableModelFactory,
        ITrayIconFactory*                 trayIconFactory,
        IRequestThread*                   requestThread,
        ISettingsEditor*                  settingsEditor,
        IAutorunEnabler*                  autorunEnabler,
        QWidget*                          parent = nullptr
    );
    ~MainWindow() override;

    MainWindow(const MainWindow& another)            = delete;
    MainWindow& operator=(const MainWindow& another) = delete;

    void closeEvent(QCloseEvent* event) override;
    void init();

    Ui::MainWindow* ui;

    QTimer requestTimer;

private:
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    IConfig*                   mConfig;
    IConfig*                   mConfigForSettingsDialog;
    ISettingsDialogFactory*    mSettingsDialogFactory;
    INotificationsTableWidget* mNotificationsTableWidget;
    ITrayIcon*                 mTrayIcon;
    IRequestThread*            mRequestThread;
    ISettingsEditor*           mSettingsEditor;
    IAutorunEnabler*           mAutorunEnabler;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
    void requestTimerTicked();

private slots:
    void on_actionSettings_triggered();
};
