#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"

#include <QChart>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QValueAxis>



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

private:
    void initRemainedMoneyChart();
    void initTotalMoneyChart();
    void handleOperation(const Operation& operation);

    QChart        mRemainedMoneyChart;
    QLineSeries   mRemainedMoneySeries;
    QDateTimeAxis mRemainedMoneyAxisX;
    QValueAxis    mRemainedMoneyAxisY;
    QChart        mTotalMoneyChart;
    QLineSeries   mTotalMoneySeries;
    QDateTimeAxis mTotalMoneyAxisX;
    QValueAxis    mTotalMoneyAxisY;
    qint64        mAxisXMin;
    qint64        mAxisXMax;
    float         mRemainedMoneyAxisYMin;
    float         mRemainedMoneyAxisYMax;
    float         mTotalMoneyAxisYMin;
    float         mTotalMoneyAxisYMax;
};
