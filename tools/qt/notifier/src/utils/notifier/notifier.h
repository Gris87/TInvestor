#pragma once



#include "src/utils/notifier/inotifier.h"

#include "src/widgets/notificationwidget/inotificationwidgetfactory.h"



class Notifier : public INotifier
{
    Q_OBJECT

public:
    explicit Notifier(INotificationWidgetFactory* notificationWidgetFactory, QObject* parent = nullptr);
    ~Notifier() override;

    Notifier(const Notifier& another)            = delete;
    Notifier& operator=(const Notifier& another) = delete;

    void setEnabled(bool enabled) override;
    void setFilter(const Filter& filter) override;
    void resetNotificationWidgets() override;
    void notificationsAdded(const QList<NotificationInfo>& notifications) override;

private:
    void createNotificationWidget(int posX, int posY, int width, int height, const QString& text);

    INotificationWidgetFactory* mNotificationWidgetFactory;
    bool                        mEnabled;
    Filter                      mFilter;
    QList<INotificationWidget*> mNotificationWidgets;

public slots:
    void notificationWidgetDestroyed(QObject* obj);
};
