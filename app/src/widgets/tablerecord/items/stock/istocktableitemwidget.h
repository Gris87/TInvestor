#pragma once



#include <QWidget>



class IStockTableItemWidget : public QWidget
{
public:
    explicit IStockTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent)
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
