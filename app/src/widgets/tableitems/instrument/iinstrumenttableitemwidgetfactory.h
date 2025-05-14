#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget.h"

#include "src/storage/user/iuserstorage.h"



class IInstrumentTableItemWidgetFactory
{
public:
    IInstrumentTableItemWidgetFactory()          = default;
    virtual ~IInstrumentTableItemWidgetFactory() = default;

    IInstrumentTableItemWidgetFactory(const IInstrumentTableItemWidgetFactory& another)            = delete;
    IInstrumentTableItemWidgetFactory& operator=(const IInstrumentTableItemWidgetFactory& another) = delete;

    virtual IInstrumentTableItemWidget* newInstance(IUserStorage* userStorage, QWidget* parent) const = 0;
};
