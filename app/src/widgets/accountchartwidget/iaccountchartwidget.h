#pragma once



#include <QChartView>

#include "src/domain/operation/operation.h"



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

    virtual void switchToYieldChart()         = 0;
    virtual void switchToMonthlyYieldChart()  = 0;
    virtual void switchToRemainedMoneyChart() = 0;
    virtual void switchToTotalMoneyChart()    = 0;

    virtual void operationsRead(const QList<Operation>& operations)  = 0;
    virtual void operationsAdded(const QList<Operation>& operations) = 0;
};
