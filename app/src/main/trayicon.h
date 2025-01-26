#pragma once



#include <QSystemTrayIcon>



class MainWindow;



class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject *parent = nullptr);
    ~TrayIcon();

    void init(MainWindow *mainWindow);
};
