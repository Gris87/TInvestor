#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget.h"

#include <QLabel>

#include "src/storage/user/iuserstorage.h"



namespace Ui
{
class StockTableItemWidget;
}



class StockTableItemWidget : public IStockTableItemWidget
{
    Q_OBJECT

public:
    explicit StockTableItemWidget(IUserStorage* userStorage, QWidget* parent = nullptr);
    ~StockTableItemWidget() override;

    StockTableItemWidget(const StockTableItemWidget& another)            = delete;
    StockTableItemWidget& operator=(const StockTableItemWidget& another) = delete;

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

    Ui::StockTableItemWidget* ui;

    QLabel* hoverTextWidget;

private:
    IUserStorage* mUserStorage;
    bool          mForQualInvestorFlag;
    QString       mFullText;
};
