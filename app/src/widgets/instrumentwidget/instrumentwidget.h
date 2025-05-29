#pragma once



#include "src/widgets/instrumentwidget/iinstrumentwidget.h"

#include <QLabel>

#include "src/storage/user/iuserstorage.h"



namespace Ui
{
class InstrumentWidget;
}



class InstrumentWidget : public IInstrumentWidget
{
    Q_OBJECT

public:
    explicit InstrumentWidget(IUserStorage* userStorage, QWidget* parent = nullptr);
    ~InstrumentWidget() override;

    InstrumentWidget(const InstrumentWidget& another)            = delete;
    InstrumentWidget& operator=(const InstrumentWidget& another) = delete;

    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void setIcon(const QIcon& icon) override;
    void setQualInvestor(bool forQualInvestorFlag) override;
    void setText(const QString& text) override;
    void setFullText(const QString& text) override;

    bool    forQualInvestorFlag() override;
    QString text() override;
    QString fullText() override;

    Ui::InstrumentWidget* ui;

    QLabel* hoverTextWidget;

private:
    IUserStorage* mUserStorage;
    bool          mForQualInvestorFlag;
    QString       mFullText;
};
