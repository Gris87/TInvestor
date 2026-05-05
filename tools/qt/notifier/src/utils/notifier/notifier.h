#pragma once



#include "src/utils/notifier/inotifier.h"

#include "src/widgets/notificationwidget/inotificationwidgetfactory.h"



class Notifier : public INotifier
{
public:
    Notifier(INotificationWidgetFactory* notificationWidgetFactory);
    ~Notifier() override;

    Notifier(const Notifier& another)            = delete;
    Notifier& operator=(const Notifier& another) = delete;

    void setEnabled(bool enabled) override;
    void setFilter(const Filter& filter) override;
    void notificationsAdded(const QList<NotificationInfo>& notifications) override;

private:
    INotificationWidgetFactory* mNotificationWidgetFactory;
    bool                        mEnabled;
    Filter                      mFilter;
};
