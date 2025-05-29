#pragma once



#include <QWidget>



class IInstrumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IInstrumentWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IInstrumentWidget() override = default;

    IInstrumentWidget(const IInstrumentWidget& another)            = delete;
    IInstrumentWidget& operator=(const IInstrumentWidget& another) = delete;

    virtual void setIcon(const QIcon& icon)                = 0;
    virtual void setQualInvestor(bool forQualInvestorFlag) = 0;
    virtual void setText(const QString& text)              = 0;
    virtual void setFullText(const QString& text)          = 0;

    virtual bool    forQualInvestorFlag() = 0;
    virtual QString text()                = 0;
    virtual QString fullText()            = 0;
};
