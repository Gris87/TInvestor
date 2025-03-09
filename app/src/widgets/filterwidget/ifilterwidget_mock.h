#pragma once



#include "src/widgets/filterwidget/ifilterwidget.h"

#include <gmock/gmock.h>



class FilterWidgetMock : public IFilterWidget
{
public:
    explicit FilterWidgetMock(QWidget* parent = nullptr) :
        IFilterWidget(parent)
    {
    }
    ~FilterWidgetMock() override = default;

    FilterWidgetMock(const FilterWidgetMock& another)            = delete;
    FilterWidgetMock& operator=(const FilterWidgetMock& another) = delete;
};
