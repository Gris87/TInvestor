#pragma once



#include "src/widgets/instrumentwidget/iinstrumentwidget.h"

#include "src/storage/user/iuserstorage.h"



class IInstrumentWidgetFactory
{
public:
    IInstrumentWidgetFactory()          = default;
    virtual ~IInstrumentWidgetFactory() = default;

    IInstrumentWidgetFactory(const IInstrumentWidgetFactory& another)            = delete;
    IInstrumentWidgetFactory& operator=(const IInstrumentWidgetFactory& another) = delete;

    virtual IInstrumentWidget* newInstance(IUserStorage* userStorage, QWidget* parent) const = 0;
};
