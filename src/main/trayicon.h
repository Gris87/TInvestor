#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>



class MainWindow;



class TrayIcon : public QSystemTrayIcon
{
public:
    explicit TrayIcon(QObject *parent = nullptr);
    ~TrayIcon();

    void init(MainWindow *mainWindow);
};



#endif // TRAYICON_H
