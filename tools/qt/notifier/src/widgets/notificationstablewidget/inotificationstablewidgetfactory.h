#pragma once



#include "src/widgets/notificationstablewidget/inotificationstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory.h"



class INotificationsTableWidgetFactory
{
public:
    INotificationsTableWidgetFactory()          = default;
    virtual ~INotificationsTableWidgetFactory() = default;

    INotificationsTableWidgetFactory(const INotificationsTableWidgetFactory& another)            = delete;
    INotificationsTableWidgetFactory& operator=(const INotificationsTableWidgetFactory& another) = delete;

    virtual INotificationsTableWidget* newInstance(
        INotificationsTableModelFactory* notificationsTableModelFactory, ISettingsEditor* settingsEditor, QWidget* parent
    ) const = 0;
};
