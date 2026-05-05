#include "src/widgets/notificationwidget/notificationwidget.h"
#include "ui_notificationwidget.h"

#include <QDebug>



NotificationWidget::NotificationWidget(QWidget* parent) :
    INotificationWidget(parent),
    ui(new Ui::NotificationWidget)
{
    qDebug() << "Create NotificationWidget";

    ui->setupUi(this);
}

NotificationWidget::~NotificationWidget()
{
    qDebug() << "Destroy NotificationWidget";

    delete ui;
}
