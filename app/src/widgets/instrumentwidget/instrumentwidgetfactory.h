#pragma once



#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory.h"



class InstrumentWidgetFactory : public IInstrumentWidgetFactory
{
public:
    InstrumentWidgetFactory();
    ~InstrumentWidgetFactory() override;

    InstrumentWidgetFactory(const InstrumentWidgetFactory& another)            = delete;
    InstrumentWidgetFactory& operator=(const InstrumentWidgetFactory& another) = delete;

    IInstrumentWidget* newInstance(IUserStorage* userStorage, QWidget* parent) const override;
};
