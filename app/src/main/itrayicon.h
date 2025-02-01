#pragma once



#include <QSystemTrayIcon>



class ITrayIcon : public QSystemTrayIcon
{
public:
    explicit ITrayIcon(QWidget *parent = nullptr) : QSystemTrayIcon(reinterpret_cast<QObject *>(parent)) {}
    virtual ~ITrayIcon() = default;

    ITrayIcon(const ITrayIcon &another) = delete;
    ITrayIcon& operator=(const ITrayIcon &another) = delete;

signals:
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void trayIconShowClicked();
    void trayIconExitClicked();
};
