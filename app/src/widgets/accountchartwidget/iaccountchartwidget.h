#pragma once



#include <QWidget>

#include "src/domain/operation/operation.h"



enum ChartType : quint8
{
    CHART_TYPE_YIELD,
    CHART_TYPE_MONTHLY_YIELD,
    CHART_TYPE_DAILY_YIELD,
    CHART_TYPE_TOTAL_MONEY,
};

enum TimeRange : quint8
{
    TIME_RANGE_ALL_TIME,
    TIME_RANGE_LAST_DAY,
    TIME_RANGE_LAST_WEEK,
    TIME_RANGE_LAST_MONTH,
    TIME_RANGE_LAST_3_MONTH,
    TIME_RANGE_COUNT,
};



class IAccountChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IAccountChartWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IAccountChartWidget() override = default;

    IAccountChartWidget(const IAccountChartWidget& another)            = delete;
    IAccountChartWidget& operator=(const IAccountChartWidget& another) = delete;

    virtual void setShowMoney(bool value) = 0;

    virtual void switchChart(ChartType chartType) = 0;
    virtual void setTimeRange(TimeRange range)    = 0;

    virtual void operationsRead(const QList<Operation>& operations)  = 0;
    virtual void operationsAdded(const QList<Operation>& operations) = 0;
};
