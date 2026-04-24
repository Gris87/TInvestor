#pragma once



#include <QMainWindow>

#include <QCloseEvent>

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
    MainWindow(ITrayIconFactory* trayIconFactory, ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
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

    ITrayIcon*       mTrayIcon;
    ISettingsEditor* mSettingsEditor;

public slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
};
