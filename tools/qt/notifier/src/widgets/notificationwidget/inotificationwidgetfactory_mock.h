#pragma once



#include "src/widgets/notificationwidget/inotificationwidgetfactory.h"

#include <gmock/gmock.h>



class NotificationWidgetFactoryMock : public INotificationWidgetFactory
{
public:
    NotificationWidgetFactoryMock() :
        INotificationWidgetFactory()
    {
    }
    ~NotificationWidgetFactoryMock() override = default;

    NotificationWidgetFactoryMock(const NotificationWidgetFactoryMock& another)            = delete;
    NotificationWidgetFactoryMock& operator=(const NotificationWidgetFactoryMock& another) = delete;

    MOCK_METHOD(INotificationWidget*, newInstance, (QWidget * parent), (const, override));
};
