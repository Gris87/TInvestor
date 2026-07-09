#include "src/widgets/notificationstablewidget/notificationstablewidgetfactory.h"

#include <QDebug>

#include "src/widgets/notificationstablewidget/notificationstablewidget.h"



NotificationsTableWidgetFactory::NotificationsTableWidgetFactory() :
    INotificationsTableWidgetFactory()
{
    qDebug() << "Create NotificationsTableWidgetFactory";
}

NotificationsTableWidgetFactory::~NotificationsTableWidgetFactory()
{
    qDebug() << "Destroy NotificationsTableWidgetFactory";
}

INotificationsTableWidget* NotificationsTableWidgetFactory::newInstance(
    INotificationsTableModelFactory* notificationsTableModelFactory,
    IHttpClient*                     httpClient,
    ISettingsEditor*                 settingsEditor,
    QWidget*                         parent
) const
{
    return new NotificationsTableWidget(notificationsTableModelFactory, httpClient, settingsEditor, parent);
}
