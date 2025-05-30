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

    virtual void setInstrumentLogo(const QIcon& icon)      = 0;
    virtual void setQualInvestor(bool forQualInvestorFlag) = 0;
    virtual void setTicker(const QString& text)            = 0;
    virtual void setFullText(const QString& text)          = 0;

    [[nodiscard]]
    virtual bool forQualInvestorFlag() const = 0;

    [[nodiscard]]
    virtual QString ticker() const = 0;

    [[nodiscard]]
    virtual QString fullText() const = 0;
};
