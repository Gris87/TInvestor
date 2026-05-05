#include "src/utils/notifier/notifier.h"

#include <QDebug>



Notifier::Notifier() :
    INotifier(),
    mEnabled(),
    mFilter()
{
    qDebug() << "Create Notifier";
}

Notifier::~Notifier()
{
    qDebug() << "Destroy Notifier";
}

void Notifier::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

void Notifier::setFilter(const Filter& filter)
{
    mFilter = filter;
}

void Notifier::notificationsAdded(const QList<NotificationInfo>& /*notifications*/)
{
    if (mEnabled)
    {
    }
}
