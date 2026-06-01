#include "src/widgets/trayicon/trayicon.h"

#include <QDebug>
#include <QMenu>
#include <QPainter>
#include <QWidget>



const QBrush BADGE_DISCONNECTED_COLOR = QBrush(QColor("#666666")); // clazy:exclude=non-pod-global-static
const QBrush BADGE_CONNECTED_COLOR    = QBrush(QColor("#009900")); // clazy:exclude=non-pod-global-static
const QBrush BADGE_BACKGROUND_COLOR   = QBrush(QColor("#FF0000")); // clazy:exclude=non-pod-global-static
const QColor BADGE_FONT_COLOR         = QColor("#FFFFFF");         // clazy:exclude=non-pod-global-static

constexpr int MAX_DISPLAY_COUNTER = 99;
constexpr int BADGE_FONT_SIZE     = 12;



TrayIcon::TrayIcon(QObject* parent) :
    ITrayIcon(parent),
    mPixmap(":/assets/images/icon.png"),
    mFilter(),
    mConnected(),
    mCounter()
{
    qDebug() << "Create TrayIcon";

    QMenu* trayIconMenu = new QMenu(reinterpret_cast<QWidget*>(parent));

    QAction* defaultAction = trayIconMenu->addAction(tr("Show"), this, SIGNAL(trayIconShowClicked()));
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("Exit"), this, SIGNAL(trayIconExitClicked()));

    trayIconMenu->setDefaultAction(defaultAction);

    setContextMenu(trayIconMenu);
    setToolTip(tr("TInvestor notifier"));

    updateIcon();
}

TrayIcon::~TrayIcon()
{
    qDebug() << "Destroy TrayIcon";
}

void TrayIcon::setFilter(const Filter& filter)
{
    mFilter = filter;
}

void TrayIcon::handleDisconnection()
{
    if (mConnected)
    {
        mConnected = false;

        updateIcon();
    }
}

void TrayIcon::handleConnection()
{
    if (!mConnected)
    {
        mConnected = true;

        updateIcon();
    }
}

void TrayIcon::resetCounter()
{
    if (mCounter != 0)
    {
        mCounter = 0;

        updateIcon();
    }
}

void TrayIcon::notificationsAdded(const QList<NotificationInfo>& notifications)
{
    int filtered = 0;

    for (const NotificationInfo& notification : notifications)
    {
        if (mFilter.isFiltered(notification))
        {
            filtered++;
        }
    }

    if (filtered > 0)
    {
        mCounter += filtered;

        updateIcon();
    }
}

void TrayIcon::updateIcon()
{
    QPixmap  pixmap = mPixmap;
    QPainter painter(&pixmap);

    if (mCounter > 0)
    {
        const int x = pixmap.width() / 4;
        const int y = pixmap.height() / 4;
        const int w = pixmap.width() - x;
        const int h = pixmap.height() - y;

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(BADGE_BACKGROUND_COLOR);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(x, y, w, h);

        painter.setPen(BADGE_FONT_COLOR);
        painter.setFont(QFont("Arial", BADGE_FONT_SIZE, QFont::Bold));
        painter.drawText(x, y, w, h, Qt::AlignCenter, QString::number(qMin(mCounter, MAX_DISPLAY_COUNTER)));
    }
    else
    {
        const int x = pixmap.width() - 12;
        const int y = pixmap.height() - 12;
        const int w = 12;
        const int h = 12;

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(mConnected ? BADGE_CONNECTED_COLOR : BADGE_DISCONNECTED_COLOR);
        painter.setPen(Qt::NoPen);
        painter.drawRect(x, y, w, h);
    }

    painter.end();

    setIcon(QIcon(pixmap));
}
