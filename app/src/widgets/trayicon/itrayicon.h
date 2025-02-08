#pragma once



#include <QSystemTrayIcon>



class ITrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit ITrayIcon(QWidget* parent = nullptr) :
        QSystemTrayIcon(reinterpret_cast<QObject*>(parent))
    {
    }
    virtual ~ITrayIcon() = default;

    ITrayIcon(const ITrayIcon& another)            = delete;
    ITrayIcon& operator=(const ITrayIcon& another) = delete;

signals:
    void trayIconShowClicked();
    void trayIconExitClicked();
};
