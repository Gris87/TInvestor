#pragma once



#include "src/widgets/notificationwidget/inotificationwidget.h"



class INotificationWidgetFactory
{
public:
    INotificationWidgetFactory()          = default;
    virtual ~INotificationWidgetFactory() = default;

    INotificationWidgetFactory(const INotificationWidgetFactory& another)            = delete;
    INotificationWidgetFactory& operator=(const INotificationWidgetFactory& another) = delete;

    virtual INotificationWidget* newInstance(QWidget* parent) const = 0;
};
