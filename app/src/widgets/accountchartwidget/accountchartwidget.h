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

    void switchToYieldChart() override;
    void switchToMonthlyYieldChart() override;
    void switchToRemainedMoneyChart() override;
    void switchToTotalMoneyChart() override;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;

private:
    void initYieldChart();
    void initMonthlyYieldChart();
    void initRemainedMoneyChart();
    void initTotalMoneyChart();
    void initChartStyle(QChart* chart, QAbstractAxis* axisX, QAbstractAxis* axisY);
    void handleOperation(const Operation& operation);

    QChart        mYieldChart;
    QLineSeries   mYieldSeries;
    QDateTimeAxis mYieldAxisX;
    QValueAxis    mYieldAxisY;
    QChart        mMonthlyYieldChart;
    QLineSeries   mMonthlyYieldSeries;
    QDateTimeAxis mMonthlyYieldAxisX;
    QValueAxis    mMonthlyYieldAxisY;
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
    float         mYieldAxisYMin;
    float         mYieldAxisYMax;
    float         mMonthlyYieldAxisYMin;
    float         mMonthlyYieldAxisYMax;
    float         mRemainedMoneyAxisYMin;
    float         mRemainedMoneyAxisYMax;
    float         mTotalMoneyAxisYMin;
    float         mTotalMoneyAxisYMax;
};
