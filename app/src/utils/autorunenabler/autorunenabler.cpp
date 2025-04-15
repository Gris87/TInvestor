#include "src/utils/autorunenabler/autorunenabler.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



const QString AUTORUN_PATH = "CurrentVersion/Run/TInvestor";



AutorunEnabler::AutorunEnabler(ISettingsEditor* autorunSettingsEditor) :
    IAutorunEnabler(),
    mAutorunSettingsEditor(autorunSettingsEditor)
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
