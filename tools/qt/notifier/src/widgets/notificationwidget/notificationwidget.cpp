#include "src/widgets/notificationwidget/notificationwidget.h"
#include "ui_notificationwidget.h"

#include <QDebug>



constexpr qint64 MS_IN_SECOND   = 1000LL;
constexpr qint64 DEATH_INTERVAL = 5 * MS_IN_SECOND; // 5 seconds



NotificationWidget::NotificationWidget(const QString& /*text*/, QWidget* parent) :
    INotificationWidget(parent),
    ui(new Ui::NotificationWidget),
    mDeathTimer()
{
    qDebug() << "Create NotificationWidget";

    ui->setupUi(this);

    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);

    mOpacityAnimation.setTargetObject(this);
    mOpacityAnimation.setPropertyName("windowOpacity");
    mOpacityAnimation.setDuration(DEATH_INTERVAL);
    mOpacityAnimation.setStartValue(1.0f);
    mOpacityAnimation.setEndValue(0.0f);

    connect(&mDeathTimer, SIGNAL(timeout()), this, SLOT(deathTimerTicked()));

    mOpacityAnimation.start();
    mDeathTimer.start(DEATH_INTERVAL);
}

NotificationWidget::~NotificationWidget()
{
    qDebug() << "Destroy NotificationWidget";

    delete ui;
}
void NotificationWidget::deathTimerTicked()
{
    deleteLater();
}
