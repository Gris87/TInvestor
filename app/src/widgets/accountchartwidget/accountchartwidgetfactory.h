#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"



class AccountChartWidgetFactory : public IAccountChartWidgetFactory
{
public:
    AccountChartWidgetFactory();
    ~AccountChartWidgetFactory() override;

    AccountChartWidgetFactory(const AccountChartWidgetFactory& another)            = delete;
    AccountChartWidgetFactory& operator=(const AccountChartWidgetFactory& another) = delete;

    IAccountChartWidget* newInstance(QWidget* parent) const override;
};
