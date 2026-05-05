#pragma once



#include "src/widgets/notificationwidget/inotificationwidget.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>



namespace Ui { class NotificationWidget; }



class NotificationWidget : public INotificationWidget
{
    Q_OBJECT

public:
    explicit NotificationWidget(const QString& text, QWidget* parent = nullptr);
    ~NotificationWidget() override;

    NotificationWidget(const NotificationWidget& another)            = delete;
    NotificationWidget& operator=(const NotificationWidget& another) = delete;

    Ui::NotificationWidget* ui;

private:
    QTimer             mDeathTimer;
    QPropertyAnimation mOpacityAnimation;

public slots:
    void deathTimerTicked();
};
