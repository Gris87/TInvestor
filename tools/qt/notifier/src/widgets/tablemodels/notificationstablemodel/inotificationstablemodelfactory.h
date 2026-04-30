#pragma once



#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodel.h"



class INotificationsTableModelFactory
{
public:
    INotificationsTableModelFactory()          = default;
    virtual ~INotificationsTableModelFactory() = default;

    INotificationsTableModelFactory(const INotificationsTableModelFactory& another)            = delete;
    INotificationsTableModelFactory& operator=(const INotificationsTableModelFactory& another) = delete;

    virtual INotificationsTableModel* newInstance(QObject* parent) const = 0;
};
