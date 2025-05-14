#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"

#include <gmock/gmock.h>



class InstrumentTableItemWidgetFactoryMock : public IInstrumentTableItemWidgetFactory
{
public:
    InstrumentTableItemWidgetFactoryMock() :
        IInstrumentTableItemWidgetFactory()
    {
    }
    ~InstrumentTableItemWidgetFactoryMock() override = default;

    InstrumentTableItemWidgetFactoryMock(const InstrumentTableItemWidgetFactoryMock& another)            = delete;
    InstrumentTableItemWidgetFactoryMock& operator=(const InstrumentTableItemWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IInstrumentTableItemWidget*, newInstance, (IUserStorage * userStorage, QWidget* parent), (const, override));
};
