#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"



class AccountChartWidget : public IAccountChartWidget
{
    Q_OBJECT

public:
    explicit AccountChartWidget(QWidget* parent = nullptr);
    ~AccountChartWidget() override;

    AccountChartWidget(const AccountChartWidget& another)            = delete;
    AccountChartWidget& operator=(const AccountChartWidget& another) = delete;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;
};
