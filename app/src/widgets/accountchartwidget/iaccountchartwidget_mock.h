#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"

#include <gmock/gmock.h>



class AccountChartWidgetMock : public IAccountChartWidget
{
public:
    explicit AccountChartWidgetMock(QWidget* parent = nullptr) :
        IAccountChartWidget(parent)
    {
    }
    ~AccountChartWidgetMock() override = default;

    AccountChartWidgetMock(const AccountChartWidgetMock& another)            = delete;
    AccountChartWidgetMock& operator=(const AccountChartWidgetMock& another) = delete;
};
