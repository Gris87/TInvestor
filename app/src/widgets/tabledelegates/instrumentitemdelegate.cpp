#include "src/widgets/tabledelegates/instrumentitemdelegate.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPainter>



constexpr int ICON_SIZE = 24;



InstrumentItemDelegate::InstrumentItemDelegate(QWidget* parent) :
    QStyledItemDelegate(parent)
{
    qDebug() << "Create InstrumentItemDelegate";
}

InstrumentItemDelegate::~InstrumentItemDelegate()
{
    qDebug() << "Destroy InstrumentItemDelegate";
}

void InstrumentItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QString instrumentId = index.data().toString();

    // TODO: Optimize
    painter->drawPixmap(
        option.rect.x(),
        option.rect.y() + (option.rect.height() - ICON_SIZE) / 2,
        ICON_SIZE,
        ICON_SIZE,
        QPixmap(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), instrumentId))
    );
}

QSize InstrumentItemDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    return QSize(ICON_SIZE, ICON_SIZE);
}
