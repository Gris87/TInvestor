#include "src/widgets/notificationwidget/notificationwidgetfactory.h"

#include <QDebug>

#include "src/widgets/notificationwidget/notificationwidget.h"



NotificationWidgetFactory::NotificationWidgetFactory() :
    INotificationWidgetFactory()
{
    qDebug() << "Create NotificationWidgetFactory";
}

NotificationWidgetFactory::~NotificationWidgetFactory()
{
    qDebug() << "Destroy NotificationWidgetFactory";
}

INotificationWidget* NotificationWidgetFactory::newInstance(const QString& text, QWidget* parent) const
{
    return new NotificationWidget(text, parent);
}
