#include "src/widgets/tabledelegates/loglevelitemdelegate.h"

#include <QDebug>
#include <QPainter>



constexpr int ICON_SIZE = 24;



LogLevelItemDelegate::LogLevelItemDelegate(QWidget* parent) :
    QStyledItemDelegate(parent),
    mIcons()
{
    qDebug() << "Create LogLevelItemDelegate";

    mIcons.append(QPixmap(":/assets/images/levels/verbose.png"));
    mIcons.append(QPixmap(":/assets/images/levels/debug.png"));
    mIcons.append(QPixmap(":/assets/images/levels/info.png"));
    mIcons.append(QPixmap(":/assets/images/levels/warning.png"));
    mIcons.append(QPixmap(":/assets/images/levels/error.png"));
}

LogLevelItemDelegate::~LogLevelItemDelegate()
{
    qDebug() << "Destroy LogLevelItemDelegate";
}

void LogLevelItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QPoint center = option.rect.center();

    painter->drawPixmap(
        center.x() - ICON_SIZE / 2, center.y() - ICON_SIZE / 2, ICON_SIZE, ICON_SIZE, mIcons.at(index.data().toInt())
    );
}

QSize LogLevelItemDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    return QSize(ICON_SIZE, ICON_SIZE);
}
