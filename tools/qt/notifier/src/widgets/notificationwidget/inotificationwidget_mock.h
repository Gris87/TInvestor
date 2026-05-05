#pragma once



#include "src/widgets/notificationwidget/inotificationwidget.h"

#include <gmock/gmock.h>



class NotificationWidgetMock : public INotificationWidget
{
    Q_OBJECT

public:
    explicit NotificationWidgetMock(QWidget* parent = nullptr) :
        INotificationWidget(parent)
    {
    }
    ~NotificationWidgetMock() override = default;

    NotificationWidgetMock(const NotificationWidgetMock& another)            = delete;
    NotificationWidgetMock& operator=(const NotificationWidgetMock& another) = delete;
};
