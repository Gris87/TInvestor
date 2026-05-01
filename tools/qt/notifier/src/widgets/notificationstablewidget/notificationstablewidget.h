#pragma once



#include "src/widgets/notificationstablewidget/inotificationstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodelfactory.h"



namespace Ui
{
class NotificationsTableWidget;
}



class NotificationsTableWidget : public INotificationsTableWidget
{
    Q_OBJECT

public:
    explicit NotificationsTableWidget(
        INotificationsTableModelFactory* notificationsTableModelFactory,
        ISettingsEditor*                 settingsEditor,
        QWidget*                         parent = nullptr
    );
    ~NotificationsTableWidget() override;

    NotificationsTableWidget(const NotificationsTableWidget& another)            = delete;
    NotificationsTableWidget& operator=(const NotificationsTableWidget& another) = delete;

    Ui::NotificationsTableWidget* ui;

    void setFilter(const Filter& filter) override;
    void notificationsRead(const QList<NotificationInfo>& notifications) override;
    void notificationsAdded(const QList<NotificationInfo>& notifications) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    ISettingsEditor*          mSettingsEditor;
    INotificationsTableModel* mNotificationsTableModel;
};
