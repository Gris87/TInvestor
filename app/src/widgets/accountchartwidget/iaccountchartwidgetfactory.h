#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"



class IAccountChartWidgetFactory
{
public:
    IAccountChartWidgetFactory()          = default;
    virtual ~IAccountChartWidgetFactory() = default;

    IAccountChartWidgetFactory(const IAccountChartWidgetFactory& another)            = delete;
    IAccountChartWidgetFactory& operator=(const IAccountChartWidgetFactory& another) = delete;

    virtual IAccountChartWidget* newInstance(QWidget* parent) const = 0;
};
