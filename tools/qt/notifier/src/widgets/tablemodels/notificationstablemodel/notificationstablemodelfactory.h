#pragma once



#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory.h"



class NotificationsTableModelFactory : public INotificationsTableModelFactory
{
public:
    NotificationsTableModelFactory();
    ~NotificationsTableModelFactory() override;

    NotificationsTableModelFactory(const NotificationsTableModelFactory& another)            = delete;
    NotificationsTableModelFactory& operator=(const NotificationsTableModelFactory& another) = delete;

    INotificationsTableModel* newInstance(QObject* parent) const override;
};
