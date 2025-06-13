#include "src/widgets/tabledelegates/instrumentitemdelegate.h"

#include <QAbstractItemView>
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>

#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel.h"



constexpr int ICON_SIZE = 24;
constexpr int SPACING   = 4;



QLabel*     InstrumentItemDelegate::hoverTextWidget = nullptr;
QModelIndex InstrumentItemDelegate::hoverIndex      = QModelIndex();



InstrumentItemDelegate::InstrumentItemDelegate(ILogosStorage* logosStorage, QWidget* parent) :
    QStyledItemDelegate(parent),
    mLogosStorage(logosStorage)
{
    qDebug() << "Create InstrumentItemDelegate";
}

InstrumentItemDelegate::~InstrumentItemDelegate()
{
    qDebug() << "Destroy InstrumentItemDelegate";

    delete hoverTextWidget;
    hoverTextWidget = nullptr;
}

void InstrumentItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    mLogosStorage->lock();
    QPixmap* logo = reinterpret_cast<QPixmap*>(index.data(LOGS_ROLE_INSTRUMENT_LOGO).toLongLong());

    painter->drawPixmap(option.rect.x(), option.rect.y() + (option.rect.height() - ICON_SIZE) / 2, ICON_SIZE, ICON_SIZE, *logo);
    mLogosStorage->unlock();

    QFontMetrics fontMetrics(option.font);

    QRect textRect = option.rect;
    textRect.setLeft(textRect.x() + ICON_SIZE + SPACING);

    if ((option.state & QStyle::State_MouseOver) == 0)
    {
        if (hoverIndex == index)
        {
            delete hoverTextWidget;
            hoverTextWidget = nullptr;
        }

        const QString ticker = index.data().toString();
        textRect             = fontMetrics.boundingRect(textRect, Qt::AlignLeft | Qt::AlignVCenter, ticker);

        if (textRect.right() > option.rect.right())
        {
            textRect.setRight(option.rect.right());
        }

        painter->drawText(textRect, ticker);
    }
    else
    {
        hoverIndex = index;

        const QString name = index.data(LOGS_ROLE_INSTRUMENT_NAME).toString();

        if (name != "")
        {
            textRect = fontMetrics.boundingRect(textRect, Qt::AlignLeft | Qt::AlignVCenter, name);

            if (hoverTextWidget == nullptr)
            {
                hoverTextWidget = new QLabel();
                hoverTextWidget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
            }

            hoverTextWidget->hide();
            hoverTextWidget->setText(name);

            const QPoint globalPos = reinterpret_cast<QAbstractItemView*>(parent())->viewport()->mapToGlobal(textRect.topLeft());
            hoverTextWidget->setGeometry(globalPos.x(), globalPos.y(), textRect.width(), textRect.height());

            hoverTextWidget->show();
        }
        else
        {
            delete hoverTextWidget;
            hoverTextWidget = nullptr;
        }
    }
}

QSize InstrumentItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QFontMetrics fontMetrics(option.font);

    const QString ticker   = index.data().toString();
    QRect         textRect = fontMetrics.boundingRect(ticker);

    return QSize(ICON_SIZE + SPACING + textRect.width(), ICON_SIZE);
}
