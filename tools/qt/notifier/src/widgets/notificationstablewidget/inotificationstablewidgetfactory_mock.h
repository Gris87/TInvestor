#pragma once



#include "src/widgets/notificationstablewidget/inotificationstablewidgetfactory.h"

#include <gmock/gmock.h>



class NotificationsTableWidgetFactoryMock : public INotificationsTableWidgetFactory
{
public:
    NotificationsTableWidgetFactoryMock() :
        INotificationsTableWidgetFactory()
    {
    }
    ~NotificationsTableWidgetFactoryMock() override = default;

    NotificationsTableWidgetFactoryMock(const NotificationsTableWidgetFactoryMock& another)            = delete;
    NotificationsTableWidgetFactoryMock& operator=(const NotificationsTableWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        INotificationsTableWidget*,
        newInstance,
        (INotificationsTableModelFactory * notificationsTableModelFactory,
         IHttpClient*     httpClient,
         ISettingsEditor* settingsEditor,
         QWidget*         parent),
        (const, override)
    );
};
