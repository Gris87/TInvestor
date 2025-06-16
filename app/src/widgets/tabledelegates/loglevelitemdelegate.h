#pragma once



#include <QStyledItemDelegate>



class LogLevelItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogLevelItemDelegate(QWidget* parent = nullptr);
    ~LogLevelItemDelegate() override;

    LogLevelItemDelegate(const LogLevelItemDelegate& another)            = delete;
    LogLevelItemDelegate& operator=(const LogLevelItemDelegate& another) = delete;

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QList<QPixmap> mIcons;
};
