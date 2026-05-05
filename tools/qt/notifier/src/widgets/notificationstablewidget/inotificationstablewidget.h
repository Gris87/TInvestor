#pragma once



#include <QWidget>

#include "src/domain/filter/filter.h"
#include "src/domain/notification/notificationinfo.h"



class INotificationsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit INotificationsTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~INotificationsTableWidget() override = default;

    INotificationsTableWidget(const INotificationsTableWidget& another)            = delete;
    INotificationsTableWidget& operator=(const INotificationsTableWidget& another) = delete;

    virtual void setFilter(const Filter& filter)                                  = 0;
    virtual void notificationsRead(const QList<NotificationInfo>& notifications)  = 0;
    virtual void notificationsAdded(const QList<NotificationInfo>& notifications) = 0;
    virtual void refreshBackground()                                              = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;

signals:
    void notificationClicked();
};
