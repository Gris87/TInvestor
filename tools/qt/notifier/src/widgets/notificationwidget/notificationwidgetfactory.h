#pragma once



#include "src/widgets/notificationwidget/inotificationwidgetfactory.h"



class NotificationWidgetFactory : public INotificationWidgetFactory
{
public:
    NotificationWidgetFactory();
    ~NotificationWidgetFactory() override;

    NotificationWidgetFactory(const NotificationWidgetFactory& another)            = delete;
    NotificationWidgetFactory& operator=(const NotificationWidgetFactory& another) = delete;

    INotificationWidget* newInstance(const QString& text, QWidget* parent) const override;
};
