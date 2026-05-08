#include "src/utils/notifier/notifier.h"

#include <QDebug>
#include <QGuiApplication>
#include <QScreen>



constexpr int MAX_AMOUNT_OF_NOTIFICATIONS = 3;
constexpr int NOTIFICATION_WIDTH          = 320;
constexpr int NOTIFICATION_HEIGHT         = 80;
constexpr int NOTIFICATION_TINY_HEIGHT    = 32;
constexpr int NOTIFICATION_GAP            = 8;



Notifier::Notifier(INotificationWidgetFactory* notificationWidgetFactory, QObject* parent) :
    INotifier(parent),
    mNotificationWidgetFactory(notificationWidgetFactory),
    mEnabled(),
    mFilter(),
    mNotificationWidgets()
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

void Notifier::resetNotificationWidgets()
{
    for (INotificationWidget* notificationWidget : std::as_const(mNotificationWidgets))
    {
        notificationWidget->deleteLater();
    }

    mNotificationWidgets.clear();
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

        const int posX = bottomRightPos.x() - NOTIFICATION_WIDTH - NOTIFICATION_GAP;
        int       posY = bottomRightPos.y() - ((NOTIFICATION_HEIGHT + NOTIFICATION_GAP) * filtered.size()) -
                   (tooMuchNotifications ? NOTIFICATION_TINY_HEIGHT + NOTIFICATION_GAP : 0) - NOTIFICATION_GAP;

        for (const NotificationInfo& notification : filtered)
        {
            posY += NOTIFICATION_GAP;

            createNotificationWidget(posX, posY, NOTIFICATION_WIDTH, NOTIFICATION_HEIGHT, notification.text);

            posY += NOTIFICATION_HEIGHT;
        }

        if (tooMuchNotifications)
        {
            posY += NOTIFICATION_GAP;

            createNotificationWidget(posX, posY, NOTIFICATION_WIDTH, NOTIFICATION_TINY_HEIGHT, "...");
        }
    }
}

void Notifier::createNotificationWidget(int posX, int posY, int width, int height, const QString& text)
{
    INotificationWidget* notificationWidget = mNotificationWidgetFactory->newInstance(text, nullptr);

    // clang-format off
    connect(notificationWidget, SIGNAL(notificationClicked()), this, SIGNAL(notificationClicked()));
    connect(notificationWidget, SIGNAL(destroyed(QObject *)),  this, SLOT(notificationWidgetDestroyed(QObject *)));
    // clang-format on

    notificationWidget->setFixedSize(width, height);
    notificationWidget->setGeometry(posX, posY, width, height);
    notificationWidget->show();

    mNotificationWidgets.append(notificationWidget);
}

void Notifier::notificationWidgetDestroyed(QObject* obj)
{
    mNotificationWidgets.removeOne(obj);
}
