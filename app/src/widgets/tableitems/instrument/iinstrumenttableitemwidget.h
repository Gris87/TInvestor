#pragma once



#include <QTableWidgetItem>
#include <QWidget>



class IInstrumentTableItemWidget : public QWidget,
                                   public QTableWidgetItem
{
    Q_OBJECT

public:
    explicit IInstrumentTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent),
        QTableWidgetItem()
    {
    }
    ~IInstrumentTableItemWidget() override = default;

    IInstrumentTableItemWidget(const IInstrumentTableItemWidget& another)            = delete;
    IInstrumentTableItemWidget& operator=(const IInstrumentTableItemWidget& another) = delete;

    virtual void setIcon(const QIcon& icon)                = 0;
    virtual void setQualInvestor(bool forQualInvestorFlag) = 0;
    virtual void setText(const QString& text)              = 0;
    virtual void setFullText(const QString& text)          = 0;

    virtual bool    forQualInvestorFlag() = 0;
    virtual QString text()                = 0;
    virtual QString fullText()            = 0;
};
