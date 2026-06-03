#include "src/widgets/notificationwidget/notificationwidget.h"
#include "ui_notificationwidget.h"

#include <QDebug>
#include <QMouseEvent>



constexpr qint64 MS_IN_SECOND   = 1000LL;
constexpr qint64 DEATH_INTERVAL = 15 * MS_IN_SECOND; // 15 seconds



NotificationWidget::NotificationWidget(const QString& text, QWidget* parent) :
    INotificationWidget(parent),
    ui(new Ui::NotificationWidget),
    mDeathTimer()
{
    qDebug() << "Create NotificationWidget";

    ui->setupUi(this);
    ui->textLabel->setText(text);

    if (text == "...")
    {
        ui->logoLabel->hide();
        ui->titleLabel->hide();
        ui->textLabel->setAlignment(Qt::AlignCenter);
    }

    setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

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

void NotificationWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit notificationClicked();
    }

    INotificationWidget::mouseReleaseEvent(event);
}

void NotificationWidget::deathTimerTicked()
{
    deleteLater();
}
