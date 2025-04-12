#pragma once



#include <QSystemTrayIcon>



class ITrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit ITrayIcon(QObject* parent = nullptr) :
        QSystemTrayIcon(parent)
    {
    }
    ~ITrayIcon() override = default;

    ITrayIcon(const ITrayIcon& another)            = delete;
    ITrayIcon& operator=(const ITrayIcon& another) = delete;

signals:
    void trayIconShowClicked();
    void trayIconExitClicked();
};
