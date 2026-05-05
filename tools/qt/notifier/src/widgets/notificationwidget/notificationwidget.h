#pragma once



#include "src/widgets/notificationwidget/inotificationwidget.h"



namespace Ui { class NotificationWidget; }



class NotificationWidget : public INotificationWidget
{
    Q_OBJECT

public:
    explicit NotificationWidget(QWidget* parent = nullptr);
    ~NotificationWidget() override;

    NotificationWidget(const NotificationWidget& another)            = delete;
    NotificationWidget& operator=(const NotificationWidget& another) = delete;

    Ui::NotificationWidget* ui;
};
