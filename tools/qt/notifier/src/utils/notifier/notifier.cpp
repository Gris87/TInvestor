#include "src/utils/notifier/notifier.h"

#include <QDebug>
#include <QGuiApplication>
#include <QScreen>



constexpr int MAX_AMOUNT_OF_NOTIFICATIONS = 3;
constexpr int NOTIFICATION_WIDTH          = 500;
constexpr int NOTIFICATION_HEIGHT         = 136;
constexpr int NOTIFICATION_TINY_HEIGHT    = 32;
constexpr int NOTIFICATION_GAP            = 8;



Notifier::Notifier(INotificationWidgetFactory* notificationWidgetFactory, QObject* parent) :
    INotifier(parent),
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
        const QScreen* screen         = QGuiApplication::primaryScreen();
        const QRect    screenGeometry = screen->availableGeometry();
        const QPoint   bottomRightPos = screenGeometry.bottomRight();

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

        const int posX = bottomRightPos.x() - (NOTIFICATION_WIDTH + NOTIFICATION_GAP);
        int       posY = bottomRightPos.y() - ((NOTIFICATION_HEIGHT + NOTIFICATION_GAP) * filtered.size()) - NOTIFICATION_GAP;

        for (const NotificationInfo& notification : filtered)
        {
            posY += NOTIFICATION_GAP;

            INotificationWidget* notificationWidget = mNotificationWidgetFactory->newInstance(notification.text, nullptr);

            connect(notificationWidget, SIGNAL(notificationClicked()), this, SLOT(notificationClicked()));

            notificationWidget->setGeometry(posX, posY, NOTIFICATION_WIDTH, NOTIFICATION_HEIGHT);
            notificationWidget->show();
        }

        if (tooMuchNotifications)
        {
            posY += NOTIFICATION_GAP;

            INotificationWidget* notificationWidget = mNotificationWidgetFactory->newInstance("...", nullptr);

            connect(notificationWidget, SIGNAL(notificationClicked()), this, SLOT(widgetClicked()));

            notificationWidget->setGeometry(posX, posY, NOTIFICATION_WIDTH, NOTIFICATION_TINY_HEIGHT);
            notificationWidget->show();
        }
    }
}

void Notifier::widgetClicked()
{
    emit notificationClicked();
}
