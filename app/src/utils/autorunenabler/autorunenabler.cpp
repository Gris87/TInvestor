#include "src/utils/autorunenabler/autorunenabler.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



const char* AUTORUN_PATH = "CurrentVersion/Run/TInvestor";



AutorunEnabler::AutorunEnabler(ISettingsEditor* autorunSettingsEditor, IDirFactory* dirFactory, IFileFactory* fileFactory) :
    IAutorunEnabler(),
    mAutorunSettingsEditor(autorunSettingsEditor),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory)
{
    qDebug() << "Create AutorunEnabler";
}

AutorunEnabler::~AutorunEnabler()
{
    qDebug() << "Destroy AutorunEnabler";
}

void AutorunEnabler::setEnabled(bool enabled)
{
    if (enabled)
    {
        enable();
    }
    else
    {
        disable();
    }
}

#ifdef Q_OS_WINDOWS
void AutorunEnabler::enable()
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const QString appPath = QDir::toNativeSeparators(qApp->applicationFilePath());

    mAutorunSettingsEditor->setValue(AUTORUN_PATH, QString("\"%1\" --autorun").arg(appPath));
}

void AutorunEnabler::disable()
{
    mAutorunSettingsEditor->remove(AUTORUN_PATH);
}
#else
void AutorunEnabler::enable()
{
    const QString appDir = qApp->applicationDirPath();

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(QDir::homePath() + "/.config/autostart");
    Q_ASSERT_X(ok, "AutorunEnabler::enable()", "Failed to create dir");

    const std::shared_ptr<IFile> file = mFileFactory->newInstance(QDir::homePath() + "/.config/autostart/TInvestor.desktop");

    ok = file->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, "AutorunEnabler::enable()", "Failed to create file");

    QString content = "";

    content += "[Desktop Entry]\n";
    content += "Type=Application\n";
    content += "Version=1.0\n";
    content += "Name=TInvestor\n";
    content += "GenericName=T-Bank Investment trading robot\n";
    content += "Comment=T-Bank Investment trading robot\n";
    content += QString("Path=%1\n").arg(appDir);
    content += QString("Exec=env LD_LIBRARY_PATH=%1 ./TInvestor --autorun\n").arg(appDir);
    content += "Icon=TInvestor\n";
    content += "Terminal=false\n";
    content += "Categories=Investment;\n";
    content += "X-GNOME-Autostart-enabled=true\n";

    file->write(content.toUtf8());
    file->close();
}

void AutorunEnabler::disable()
{
    const std::shared_ptr<IFile> file = mFileFactory->newInstance(QDir::homePath() + "/.config/autostart/TInvestor.desktop");

    if (file->exists())
    {
        const bool ok = file->remove();
        Q_ASSERT_X(ok, "AutorunEnabler::disable()", "Failed to delete file");
    }
}
#endif
