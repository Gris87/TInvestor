#include "src/widgets/notificationstablewidget/notificationstablewidget.h"
#include "ui_notificationstablewidget.h"

#include <QDebug>



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[NOTIFICATIONS_COLUMN_COUNT] = {110, 103, 900};
#else
const int COLUMN_WIDTHS[NOTIFICATIONS_COLUMN_COUNT] = {145, 98, 1582};
#endif



NotificationsTableWidget::NotificationsTableWidget(
    INotificationsTableModelFactory* notificationsTableModelFactory, ISettingsEditor* settingsEditor, QWidget* parent
) :
    INotificationsTableWidget(parent),
    ui(new Ui::NotificationsTableWidget),
    mSettingsEditor(settingsEditor),
    mNotificationsTableModel()
{
    qDebug() << "Create NotificationsTableWidget";

    ui->setupUi(this);

    mNotificationsTableModel = notificationsTableModelFactory->newInstance(this);

    ui->tableView->setModel(mNotificationsTableModel);
    ui->tableView->sortByColumn(NOTIFICATIONS_TIME_COLUMN, Qt::DescendingOrder);
}

NotificationsTableWidget::~NotificationsTableWidget()
{
    qDebug() << "Destroy NotificationsTableWidget";

    delete ui;
}

void NotificationsTableWidget::setFilter(const Filter& filter)
{
    mNotificationsTableModel->setFilter(filter);
}

void NotificationsTableWidget::notificationsRead(const QList<NotificationInfo>& notifications)
{
    mNotificationsTableModel->notificationsRead(notifications);
    ui->tableView->sortByColumn(NOTIFICATIONS_TIME_COLUMN, Qt::DescendingOrder);
}

void NotificationsTableWidget::notificationsAdded(const QList<NotificationInfo>& notifications)
{
    mNotificationsTableModel->notificationsAdded(notifications);
}

void NotificationsTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Time", ui->tableView->columnWidth(NOTIFICATIONS_TIME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Type", ui->tableView->columnWidth(NOTIFICATIONS_TYPE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Text", ui->tableView->columnWidth(NOTIFICATIONS_TEXT_COLUMN));
    // clang-format on
}

void NotificationsTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableView->setColumnWidth(NOTIFICATIONS_TIME_COLUMN, mSettingsEditor->value(type + "/columnWidth_Time", COLUMN_WIDTHS[NOTIFICATIONS_TIME_COLUMN]).toInt());
    ui->tableView->setColumnWidth(NOTIFICATIONS_TYPE_COLUMN, mSettingsEditor->value(type + "/columnWidth_Type", COLUMN_WIDTHS[NOTIFICATIONS_TYPE_COLUMN]).toInt());
    ui->tableView->setColumnWidth(NOTIFICATIONS_TEXT_COLUMN, mSettingsEditor->value(type + "/columnWidth_Text", COLUMN_WIDTHS[NOTIFICATIONS_TEXT_COLUMN]).toInt());
    // clang-format on
}
