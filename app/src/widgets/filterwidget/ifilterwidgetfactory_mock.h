#pragma once



#include "src/widgets/filterwidget/ifilterwidgetfactory.h"

#include <gmock/gmock.h>



class FilterWidgetFactoryMock : public IFilterWidgetFactory
{
public:
    FilterWidgetFactoryMock() :
        IFilterWidgetFactory()
    {
    }
    ~FilterWidgetFactoryMock() override = default;

    FilterWidgetFactoryMock(const FilterWidgetFactoryMock& another)            = delete;
    FilterWidgetFactoryMock& operator=(const FilterWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IFilterWidget*, newInstance, (QWidget * parent), (override));
};
