#pragma once



#include <QChartView>

#include "src/domain/operation/operation.h"



enum ChartType : quint8
{
    CHART_TYPE_YIELD,
    CHART_TYPE_MONTHLY_YIELD,
    CHART_TYPE_REMAINED_MONEY,
    CHART_TYPE_TOTAL_MONEY,
};



class IAccountChartWidget : public QChartView
{
    Q_OBJECT

public:
    explicit IAccountChartWidget(QWidget* parent = nullptr) :
        QChartView(parent)
    {
    }
    ~IAccountChartWidget() override = default;

    IAccountChartWidget(const IAccountChartWidget& another)            = delete;
    IAccountChartWidget& operator=(const IAccountChartWidget& another) = delete;

    virtual void switchChart(ChartType chartType) = 0;

    virtual void operationsRead(const QList<Operation>& operations)  = 0;
    virtual void operationsAdded(const QList<Operation>& operations) = 0;
};
