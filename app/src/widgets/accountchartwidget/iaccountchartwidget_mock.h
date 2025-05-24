#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"

#include <gmock/gmock.h>



class AccountChartWidgetMock : public IAccountChartWidget
{
    Q_OBJECT

public:
    explicit AccountChartWidgetMock(QWidget* parent = nullptr) :
        IAccountChartWidget(parent)
    {
    }
    ~AccountChartWidgetMock() override = default;

    AccountChartWidgetMock(const AccountChartWidgetMock& another)            = delete;
    AccountChartWidgetMock& operator=(const AccountChartWidgetMock& another) = delete;

    MOCK_METHOD(void, switchChart, (ChartType chartType), (override));

    MOCK_METHOD(void, operationsRead, (const QList<Operation>& operations), (override));
    MOCK_METHOD(void, operationsAdded, (const QList<Operation>& operations), (override));
};
