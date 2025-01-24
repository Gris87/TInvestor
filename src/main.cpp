#include <QApplication>

#include <QDebug>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTranslator>

#include "src/logger/logger.h"
#include "src/main/mainwindow.h"

#ifdef QT_NO_SYSTEMTRAYICON
#error "QSystemTrayIcon is not supported on this platform"
#endif



int main(int argc, char *argv[])
{
    Logger::init();
    qInfo() << "START";

    QApplication app(argc, argv);

    Q_INIT_RESOURCE(Resources);

    QTranslator translator;
    if (!translator.load(":/assets/translations/language_ru.qm"))
    {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Localization"),
            QObject::tr("Failed to apply localization"),
            QMessageBox::Close
        );

        return 1;
    }

    app.installTranslator(&translator);
    qDebug() << "Localization applied";

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Systray"),
            QObject::tr("Failed to detect any system tray on this system"),
            QMessageBox::Close
        );

        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    MainWindow mainWindow;
    mainWindow.init();

    int res = app.exec();

    qInfo() << "END";

    return res;
}
