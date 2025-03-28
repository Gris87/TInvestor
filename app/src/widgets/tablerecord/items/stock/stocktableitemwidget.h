#pragma once



#include "src/widgets/tablerecord/items/stock/istocktableitemwidget.h"

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
    ~StockTableItemWidget();

    StockTableItemWidget(const StockTableItemWidget& another)            = delete;
    StockTableItemWidget& operator=(const StockTableItemWidget& another) = delete;

    void setIcon(const QIcon& icon) override;
    void setQualInvestor(bool forQualInvestorFlag) override;
    void setText(const QString& text) override;
    void setFullText(const QString& text) override;

    bool    forQualInvestorFlag() override;
    QString text() override;
    QString fullText() override;

    Ui::StockTableItemWidget* ui;

private:
    IUserStorage* mUserStorage;
    float         mForQualInvestorFlag;
};
