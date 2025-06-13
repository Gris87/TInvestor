#pragma once



#include <QStyledItemDelegate>



class InstrumentItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit InstrumentItemDelegate(QWidget* parent = nullptr);
    ~InstrumentItemDelegate() override;

    InstrumentItemDelegate(const InstrumentItemDelegate& another)            = delete;
    InstrumentItemDelegate& operator=(const InstrumentItemDelegate& another) = delete;

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
