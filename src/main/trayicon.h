#ifndef TRAYICON_H
#define TRAYICON_H

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



#endif // TRAYICON_H
