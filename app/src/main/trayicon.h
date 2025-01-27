#pragma once



#include <QSystemTrayIcon>



class MainWindow;



class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject *parent = nullptr);
    ~TrayIcon();

    TrayIcon(const TrayIcon &another) = delete;
    TrayIcon& operator=(const TrayIcon &another) = delete;

    void init(MainWindow *mainWindow);
};
