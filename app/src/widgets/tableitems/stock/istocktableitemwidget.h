#pragma once



#include <QTableWidgetItem>
#include <QWidget>



class IStockTableItemWidget : public QWidget,
                              public QTableWidgetItem
{
public:
    explicit IStockTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent),
        QTableWidgetItem()
    {
    }
    virtual ~IStockTableItemWidget() = default;

    IStockTableItemWidget(const IStockTableItemWidget& another)            = delete;
    IStockTableItemWidget& operator=(const IStockTableItemWidget& another) = delete;

    virtual void setIcon(const QIcon& icon)                = 0;
    virtual void setQualInvestor(bool forQualInvestorFlag) = 0;
    virtual void setText(const QString& text)              = 0;
    virtual void setFullText(const QString& text)          = 0;

    virtual bool    forQualInvestorFlag() = 0;
    virtual QString text()                = 0;
    virtual QString fullText()            = 0;
};
