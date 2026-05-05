#pragma once



#include "src/widgets/trayicon/itrayicon.h"



class TrayIcon : public ITrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject* parent = nullptr);
    ~TrayIcon() override;

    TrayIcon(const TrayIcon& another)            = delete;
    TrayIcon& operator=(const TrayIcon& another) = delete;

    void setFilter(const Filter& filter) override;
    void resetCounter() override;
    void notificationsAdded(const QList<NotificationInfo>& notifications) override;

private:
    Filter mFilter;
    int    mCounter;
};
