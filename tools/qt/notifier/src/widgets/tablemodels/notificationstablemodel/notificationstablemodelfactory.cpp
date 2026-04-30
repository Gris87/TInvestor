#include "src/widgets/tablemodels/notificationstablemodel/notificationstablemodelfactory.h"

#include <QDebug>

#include "src/widgets/tablemodels/notificationstablemodel/notificationstablemodel.h"



NotificationsTableModelFactory::NotificationsTableModelFactory() :
    INotificationsTableModelFactory()
{
    qDebug() << "Create NotificationsTableModelFactory";
}

NotificationsTableModelFactory::~NotificationsTableModelFactory()
{
    qDebug() << "Destroy NotificationsTableModelFactory";
}

INotificationsTableModel* NotificationsTableModelFactory::newInstance(QObject* parent) const
{
    return new NotificationsTableModel(parent);
}
