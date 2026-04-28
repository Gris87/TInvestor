#include <QApplication>

#include <QDebug>
#include <QMessageBox>
#include <QStyleFactory>
#include <QSystemTrayIcon>
#include <QTextStream>
#include <QThread>
#include <QTranslator>

#include "src/config/config.h"
#include "src/dialogs/settingsdialog/settingsdialogfactory.h"
#include "src/main/mainwindow.h"
#include "src/threads/request/requestthread.h"
#include "src/utils/autorunenabler/autorunenabler.h"
#include "src/utils/fs/dir/dirfactory.h"
#include "src/utils/fs/file/filefactory.h"
#include "src/utils/logger/logger.h"
#include "src/utils/settingseditor/settingseditor.h"
#include "src/utils/style/darkpalette.h"
#include "src/widgets/trayicon/trayiconfactory.h"

#ifdef QT_NO_SYSTEMTRAYICON
#error "QSystemTrayIcon is not supported on this platform"
#endif



const char* const VERSION  = "v1.4.0";
const char* const GIT_HASH = "DEV_BUILD";

constexpr int    DETECT_TRAY_ATTEMPTS = 5;
constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 DETECT_TRAY_DELAY    = MS_IN_SECOND; // 1 second



static bool isHelpNeeded(int argc, char* argv[])
{
    const QStringList helpArguments{"--help", "-h", "-?"};

    for (int i = 0; i < argc; ++i)
    {
        if (helpArguments.contains(argv[i]))
        {
            return true;
        }
    }

    return false;
}

static bool isVersionNeeded(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        if (QString(argv[i]) == "--version")
        {
            return true;
        }
    }

    return false;
}

static bool isLogToFileNeeded(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        if (QString(argv[i]) == "--log-to-file")
        {
            return true;
        }
    }

    return false;
}

static int runApplication(QApplication* app)
{
    app->setStyle(QStyleFactory::create("Fusion"));
    app->setPalette(DarkPalette());

    Q_INIT_RESOURCE(Resources);

    QTranslator translator;
    if (!translator.load(":/assets/translations/language_ru.qm"))
    {
        QMessageBox::critical(
            nullptr, QObject::tr("Localization"), QObject::tr("Failed to apply localization"), QMessageBox::Close
        );

        return 1;
    }

    app->installTranslator(&translator);
    qDebug() << "Localization applied";

    bool trayFound = false;

    for (int i = 0; i < DETECT_TRAY_ATTEMPTS; ++i)
    {
        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
            trayFound = true;

            break;
        }

        QThread::msleep(DETECT_TRAY_DELAY);
    }

    if (!trayFound)
    {
        QMessageBox::critical(
            nullptr, QObject::tr("Systray"), QObject::tr("Failed to detect any system tray on this system"), QMessageBox::Close
        );

        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    SettingsDialogFactory settingsDialogFactory;

    TrayIconFactory trayIconFactory;
    DirFactory      dirFactory;
    FileFactory     fileFactory;

    SettingsEditor settingsEditor("GrisCom", "TInvestorNotifier");
    SettingsEditor autorunSettingsEditor("Microsoft", "Windows");
    AutorunEnabler autorunEnabler(&autorunSettingsEditor, &dirFactory, &fileFactory);

    Config config;
    Config configForSettingsDialog;

    RequestThread requestThread(&config);

    MainWindow mainWindow(
        &config,
        &configForSettingsDialog,
        &settingsDialogFactory,
        &trayIconFactory,
        &requestThread,
        &settingsEditor,
        &autorunEnabler
    );
    mainWindow.init();

    qInfo() << "UP and Running";

    if (!app->arguments().contains("--autorun"))
    {
        mainWindow.show();
    }

    return app->exec();
}

int main(int argc, char* argv[])
{
    if (isHelpNeeded(argc, argv))
    {
        QTextStream out(stdout);

        out << "Usage:\n";
        out << "  TInvestor [OPTIONS]\n";
        out << "\n";
        out << "Options:\n";
        out << "  -h            - Display this message\n";
        out << "  --help        - Display this message\n";
        out << "  -?            - Display this message\n";
        out << "  --version     - Prints version info\n";
        out << "  --log-to-file - Write logs to notifier_logs.txt file\n";
        out << "  --autorun     - Run application in hidden mode\n";

        return 0;
    }

    if (isVersionNeeded(argc, argv))
    {
        QTextStream out(stdout);

        out << VERSION << " - " << GIT_HASH << Qt::endl;

        return 0;
    }

    Logger::init();

    if (isLogToFileNeeded(argc, argv))
    {
        Logger::enableLogToFile();
    }

    QApplication app(argc, argv);

    qInfo() << "START";

    const int res = runApplication(&app);

    qInfo() << "END";

    return res;
}
