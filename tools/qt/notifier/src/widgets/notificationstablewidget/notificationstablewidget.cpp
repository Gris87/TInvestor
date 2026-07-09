#include "src/widgets/notificationstablewidget/notificationstablewidget.h"
#include "ui_notificationstablewidget.h"

#include <QDebug>
#include <QDesktopServices>
#include <QMouseEvent>

#include "src/widgets/tablemodels/modelroles.h"



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[NOTIFICATIONS_COLUMN_COUNT] = {110, 128, 1590};
#else
const int COLUMN_WIDTHS[NOTIFICATIONS_COLUMN_COUNT] = {145, 123, 1590};
#endif



NotificationsTableWidget::NotificationsTableWidget(
    INotificationsTableModelFactory* notificationsTableModelFactory,
    IHttpClient*                     httpClient,
    ISettingsEditor*                 settingsEditor,
    QWidget*                         parent
) :
    INotificationsTableWidget(parent),
    ui(new Ui::NotificationsTableWidget),
    mHttpClient(httpClient),
    mSettingsEditor(settingsEditor),
    mNotificationsTableModel()
{
    qDebug() << "Create NotificationsTableWidget";

    ui->setupUi(this);

    mNotificationsTableModel = notificationsTableModelFactory->newInstance(this);

    ui->tableView->setModel(mNotificationsTableModel);
    ui->tableView->sortByColumn(NOTIFICATIONS_TIME_COLUMN, Qt::DescendingOrder);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->viewport()->installEventFilter(this);
}

NotificationsTableWidget::~NotificationsTableWidget()
{
    qDebug() << "Destroy NotificationsTableWidget";

    delete ui;
}

bool NotificationsTableWidget::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonRelease)
    {
        const QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        const QModelIndex  index      = ui->tableView->indexAt(mouseEvent->pos());

        const QString url = index.isValid() && index.column() == NOTIFICATIONS_TEXT_COLUMN ? index.data(ROLE_URL).toString() : "";

        if (url != "")
        {
            ui->tableView->setCursor(Qt::PointingHandCursor);

            if (event->type() == QEvent::MouseButtonRelease && mouseEvent->button() == Qt::LeftButton)
            {
                mHttpClient->openInBrowser(QUrl(url));
            }
        }
        else
        {
            ui->tableView->setCursor(Qt::ArrowCursor);
        }
    }

    return INotificationsTableWidget::eventFilter(object, event);
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

void NotificationsTableWidget::refreshBackground()
{
    mNotificationsTableModel->refreshBackground();
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
