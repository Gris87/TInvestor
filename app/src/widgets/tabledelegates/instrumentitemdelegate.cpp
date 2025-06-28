#include "src/widgets/tabledelegates/instrumentitemdelegate.h"

#include <QAbstractItemView>
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>

#include "src/domain/logo/logo.h"
#include "src/widgets/tablemodels/modelroles.h"



constexpr int LOGO_ICON_SIZE = 24;
constexpr int LOCK_ICON_SIZE = 16;
constexpr int SPACING        = 4;



QLabel*     InstrumentItemDelegate::hoverTextWidget = nullptr;
QModelIndex InstrumentItemDelegate::hoverIndex      = QModelIndex();



InstrumentItemDelegate::InstrumentItemDelegate(QWidget* parent) :
    QStyledItemDelegate(parent),
    mLockedPixmap(":/assets/images/lock.png")
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
    Logo* logo = reinterpret_cast<Logo*>(index.data(ROLE_INSTRUMENT_LOGO).toLongLong()); // NOLINT(performance-no-int-to-ptr)

    if (logo != nullptr)
    {
        logo->readLock();
        painter->drawPixmap(
            option.rect.x(),
            option.rect.y() + ((option.rect.height() - LOGO_ICON_SIZE) / 2),
            LOGO_ICON_SIZE,
            LOGO_ICON_SIZE,
            logo->pixmap
        );
        logo->readUnlock();
    }

    qint8 offset = LOGO_ICON_SIZE + SPACING;

    const bool locked = index.data(ROLE_INSTRUMENT_LOCKED).toBool();

    if (locked)
    {
        painter->drawPixmap(
            option.rect.x() + offset,
            option.rect.y() + ((option.rect.height() - LOCK_ICON_SIZE) / 2),
            LOCK_ICON_SIZE,
            LOCK_ICON_SIZE,
            mLockedPixmap
        );

        offset += LOCK_ICON_SIZE + SPACING;
    }

    const QFontMetrics fontMetrics(option.font);

    QRect textRect = option.rect;
    textRect.setLeft(textRect.x() + offset);

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

        painter->drawText(textRect, 0, ticker);
    }
    else
    {
        hoverIndex = index;

        const QString name = index.data(ROLE_INSTRUMENT_NAME).toString();

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
    qint8 offset = LOGO_ICON_SIZE + SPACING;

    const bool locked = index.data(ROLE_INSTRUMENT_LOCKED).toBool();

    if (locked)
    {
        offset += LOCK_ICON_SIZE + SPACING;
    }

    const QFontMetrics fontMetrics(option.font);

    const QString ticker   = index.data().toString();
    const QRect   textRect = fontMetrics.boundingRect(ticker);

    return QSize(offset + textRect.width() + SPACING, LOGO_ICON_SIZE);
}
