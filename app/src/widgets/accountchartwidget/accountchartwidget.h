#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"

#include <QChart>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QTimer>
#include <QValueAxis>

#include "src/widgets/accountchartwidget/charttooltip.h"



class AccountChartWidget : public IAccountChartWidget
{
    Q_OBJECT

public:
    explicit AccountChartWidget(QWidget* parent = nullptr);
    ~AccountChartWidget() override;

    AccountChartWidget(const AccountChartWidget& another)            = delete;
    AccountChartWidget& operator=(const AccountChartWidget& another) = delete;

    QTimer tooltipHideTimer;

    void zoom(double factor);

    bool eventFilter(QObject* object, QEvent* event) override;

    void switchChart(ChartType chartType) override;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;

public slots:
    void seriesHovered(QPointF point, bool state);
    void tooltipHideTimerTicked();

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
    QPointF       mTargetScenePos;
    QPointF       mTargetViewportPos;
    ChartTooltip* mTooltip;
};
