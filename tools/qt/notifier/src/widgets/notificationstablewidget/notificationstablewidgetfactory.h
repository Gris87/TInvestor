#pragma once



#include "src/widgets/notificationstablewidget/inotificationstablewidgetfactory.h"



class NotificationsTableWidgetFactory : public INotificationsTableWidgetFactory
{
public:
    NotificationsTableWidgetFactory();
    ~NotificationsTableWidgetFactory() override;

    NotificationsTableWidgetFactory(const NotificationsTableWidgetFactory& another)            = delete;
    NotificationsTableWidgetFactory& operator=(const NotificationsTableWidgetFactory& another) = delete;

    INotificationsTableWidget* newInstance(
        INotificationsTableModelFactory* notificationsTableModelFactory, ISettingsEditor* settingsEditor, QWidget* parent
    ) const override;
};
