#include "src/widgets/trayicon/trayicon.h"

#include <QDebug>
#include <QMenu>
#include <QPainter>
#include <QWidget>



const QBrush BADGE_BACKGROUND_COLOR = QBrush(QColor("#FF0000")); // clazy:exclude=non-pod-global-static
const QColor BADGE_FONT_COLOR       = QColor("#FFFFFF");         // clazy:exclude=non-pod-global-static

constexpr int MAX_DISPLAY_COUNTER = 99;



TrayIcon::TrayIcon(QObject* parent) :
    ITrayIcon(parent),
    mFilter(),
    mCounter()
{
    qDebug() << "Create TrayIcon";

    QMenu* trayIconMenu = new QMenu(reinterpret_cast<QWidget*>(parent));

    QAction* defaultAction = trayIconMenu->addAction(tr("Show"), this, SIGNAL(trayIconShowClicked()));
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("Exit"), this, SIGNAL(trayIconExitClicked()));

    trayIconMenu->setDefaultAction(defaultAction);

    setIcon(QIcon(":/assets/images/icon.png"));
    setContextMenu(trayIconMenu);
    setToolTip(tr("TInvestor notifier"));
}

TrayIcon::~TrayIcon()
{
    qDebug() << "Destroy TrayIcon";
}

void TrayIcon::setFilter(const Filter& filter)
{
    mFilter = filter;
}

void TrayIcon::resetCounter()
{
    if (mCounter != 0)
    {
        mCounter = 0;

        setIcon(QIcon(":/assets/images/icon.png"));
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

        QPixmap  pixmap(":/assets/images/icon.png");
        QPainter painter(&pixmap);

        const int x = pixmap.width() / 4;
        const int y = pixmap.height() / 4;
        const int w = pixmap.width() - x;
        const int h = pixmap.height() - y;

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(BADGE_BACKGROUND_COLOR);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(x, y, w, h);

        painter.setPen(BADGE_FONT_COLOR);
        painter.setFont(QFont("Arial", 48, QFont::Bold));
        painter.drawText(x, y, w, h, Qt::AlignCenter, QString::number(qMin(mCounter, MAX_DISPLAY_COUNTER)));
        painter.end();

        setIcon(QIcon(pixmap));
    }
}
