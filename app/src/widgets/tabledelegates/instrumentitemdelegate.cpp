#include "src/widgets/tabledelegates/instrumentitemdelegate.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPainter>

#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel.h"



constexpr int ICON_SIZE = 24;



InstrumentItemDelegate::InstrumentItemDelegate(ILogosStorage* logosStorage, QWidget* parent) :
    QStyledItemDelegate(parent),
    mLogosStorage(logosStorage)
{
    qDebug() << "Create InstrumentItemDelegate";
}

InstrumentItemDelegate::~InstrumentItemDelegate()
{
    qDebug() << "Destroy InstrumentItemDelegate";
}

void InstrumentItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    mLogosStorage->lock();
    QPixmap* logo = reinterpret_cast<QPixmap*>(index.data(LOGS_ROLE_INSTRUMENT_LOGO).data());

    painter->drawPixmap(option.rect.x(), option.rect.y() + (option.rect.height() - ICON_SIZE) / 2, ICON_SIZE, ICON_SIZE, *logo);
    mLogosStorage->unlock();
}

QSize InstrumentItemDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    return QSize(ICON_SIZE, ICON_SIZE);
}
