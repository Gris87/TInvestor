#pragma once



#include <QMainWindow>

#include <QCloseEvent>

#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"
#include "src/utils/autorunenabler/iautorunenabler.h"
#include "src/utils/settingseditor/isettingseditor.h"
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
        IConfig*                config,
        IConfig*                configForSettingsDialog,
        ISettingsDialogFactory* settingsDialogFactory,
        ITrayIconFactory*       trayIconFactory,
        ISettingsEditor*        settingsEditor,
        IAutorunEnabler*        autorunEnabler,
        QWidget*                parent = nullptr
    );
    ~MainWindow() override;

    MainWindow(const MainWindow& another)            = delete;
    MainWindow& operator=(const MainWindow& another) = delete;

    void closeEvent(QCloseEvent* event) override;
    void init();

    Ui::MainWindow* ui;

private:
    void applyConfig();
    void saveWindowState();
    void loadWindowState();

    IConfig*                mConfig;
    IConfig*                mConfigForSettingsDialog;
    ISettingsDialogFactory* mSettingsDialogFactory;
    ITrayIcon*              mTrayIcon;
    ISettingsEditor*        mSettingsEditor;
    IAutorunEnabler*        mAutorunEnabler;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();

private slots:
    void on_actionSettings_triggered();
};
