#pragma once



#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory.h"

#include <gmock/gmock.h>



class NotificationsTableModelFactoryMock : public INotificationsTableModelFactory
{
public:
    NotificationsTableModelFactoryMock() :
        INotificationsTableModelFactory()
    {
    }
    ~NotificationsTableModelFactoryMock() override = default;

    NotificationsTableModelFactoryMock(const NotificationsTableModelFactoryMock& another)            = delete;
    NotificationsTableModelFactoryMock& operator=(const NotificationsTableModelFactoryMock& another) = delete;

    MOCK_METHOD(INotificationsTableModel*, newInstance, (QObject * parent), (const, override));
};
