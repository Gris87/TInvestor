#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget.h"

#include <QLabel>

#include "src/storage/user/iuserstorage.h"



namespace Ui
{
class InstrumentTableItemWidget;
}



class InstrumentTableItemWidget : public IInstrumentTableItemWidget
{
    Q_OBJECT

public:
    explicit InstrumentTableItemWidget(IUserStorage* userStorage, QWidget* parent = nullptr);
    ~InstrumentTableItemWidget() override;

    InstrumentTableItemWidget(const InstrumentTableItemWidget& another)            = delete;
    InstrumentTableItemWidget& operator=(const InstrumentTableItemWidget& another) = delete;

    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void setIcon(const QIcon& icon) override;
    void setQualInvestor(bool forQualInvestorFlag) override;
    void setText(const QString& text) override;
    void setFullText(const QString& text) override;

    bool    forQualInvestorFlag() override;
    QString text() override;
    QString fullText() override;

    bool operator<(const QTableWidgetItem& another) const override;

    Ui::InstrumentTableItemWidget* ui;

    QLabel* hoverTextWidget;

private:
    IUserStorage* mUserStorage;
    bool          mForQualInvestorFlag;
    QString       mFullText;
};
