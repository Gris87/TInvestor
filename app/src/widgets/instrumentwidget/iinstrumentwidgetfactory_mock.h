#pragma once



#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory.h"

#include <gmock/gmock.h>



class InstrumentWidgetFactoryMock : public IInstrumentWidgetFactory
{
public:
    InstrumentWidgetFactoryMock() :
        IInstrumentWidgetFactory()
    {
    }
    ~InstrumentWidgetFactoryMock() override = default;

    InstrumentWidgetFactoryMock(const InstrumentWidgetFactoryMock& another)            = delete;
    InstrumentWidgetFactoryMock& operator=(const InstrumentWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IInstrumentWidget*, newInstance, (IUserStorage * userStorage, QWidget* parent), (const, override));
};
