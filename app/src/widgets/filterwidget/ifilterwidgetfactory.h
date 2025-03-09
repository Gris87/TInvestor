#pragma once



#include "src/widgets/filterwidget/ifilterwidget.h"



class IFilterWidgetFactory
{
public:
    IFilterWidgetFactory()
    {
    }
    virtual ~IFilterWidgetFactory() = default;

    IFilterWidgetFactory(const IFilterWidgetFactory& another)            = delete;
    IFilterWidgetFactory& operator=(const IFilterWidgetFactory& another) = delete;

    virtual IFilterWidget* newInstance(QWidget* parent) = 0;
};
