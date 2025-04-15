#include "src/utils/autorunenabler/autorunenabler.h"

#include <QDebug>



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
}

void AutorunEnabler::disable()
{
}
