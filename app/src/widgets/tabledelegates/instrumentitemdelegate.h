#pragma once



#include <QStyledItemDelegate>

#include <QLabel>

#include "src/storage/logos/ilogosstorage.h"



class InstrumentItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit InstrumentItemDelegate(ILogosStorage* logosStorage, QWidget* parent = nullptr);
    ~InstrumentItemDelegate() override;

    InstrumentItemDelegate(const InstrumentItemDelegate& another)            = delete;
    InstrumentItemDelegate& operator=(const InstrumentItemDelegate& another) = delete;

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

    static QLabel*     hoverTextWidget;
    static QModelIndex hoverIndex;

private:
    ILogosStorage* mLogosStorage;
};
