#pragma once



#include "src/utils/notifier/inotifier.h"



class Notifier : public INotifier
{
public:
    Notifier();
    ~Notifier() override;

    Notifier(const Notifier& another)            = delete;
    Notifier& operator=(const Notifier& another) = delete;

    void setEnabled(bool enabled) override;
    void setFilter(const Filter& filter) override;
    void notificationsAdded(const QList<NotificationInfo>& notifications) override;

private:
    bool   mEnabled;
    Filter mFilter;
};
