#pragma once



#include "src/widgets/filterwidget/ifilterwidgetfactory.h"



class FilterWidgetFactory : public IFilterWidgetFactory
{
public:
    FilterWidgetFactory();
    ~FilterWidgetFactory();

    FilterWidgetFactory(const FilterWidgetFactory& another)            = delete;
    FilterWidgetFactory& operator=(const FilterWidgetFactory& another) = delete;

    IFilterWidget* newInstance(QWidget* parent) override;
};
