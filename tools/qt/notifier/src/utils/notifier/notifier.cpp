#include "src/utils/notifier/notifier.h"

#include <QDebug>



constexpr int MAX_AMOUNT_OF_NOTIFICATIONS = 3;



Notifier::Notifier(INotificationWidgetFactory* notificationWidgetFactory) :
    INotifier(),
    mNotificationWidgetFactory(notificationWidgetFactory),
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

void Notifier::notificationsAdded(const QList<NotificationInfo>& notifications)
{
    if (mEnabled)
    {
        QList<NotificationInfo> filtered;
        bool                    tooMuchNotifications = false;

        for (const NotificationInfo& notification : notifications)
        {
            if (mFilter.isFiltered(notification))
            {
                tooMuchNotifications = filtered.size() >= MAX_AMOUNT_OF_NOTIFICATIONS;

                if (tooMuchNotifications)
                {
                    break;
                }

                filtered.append(notification);
            }
        }

        for (const NotificationInfo& notification : filtered)
        {
            INotificationWidget* notificationWidget = mNotificationWidgetFactory->newInstance(notification.text, nullptr);

            notificationWidget->show();
        }

        if (tooMuchNotifications)
        {
        }
    }
}
