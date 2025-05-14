#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"



class InstrumentTableItemWidgetFactory : public IInstrumentTableItemWidgetFactory
{
public:
    InstrumentTableItemWidgetFactory();
    ~InstrumentTableItemWidgetFactory() override;

    InstrumentTableItemWidgetFactory(const InstrumentTableItemWidgetFactory& another)            = delete;
    InstrumentTableItemWidgetFactory& operator=(const InstrumentTableItemWidgetFactory& another) = delete;

    IInstrumentTableItemWidget* newInstance(IUserStorage* userStorage, QWidget* parent) const override;
};
