#pragma once



#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidget.h"



class ILogLevelTableItemWidgetFactory
{
public:
    ILogLevelTableItemWidgetFactory()          = default;
    virtual ~ILogLevelTableItemWidgetFactory() = default;

    ILogLevelTableItemWidgetFactory(const ILogLevelTableItemWidgetFactory& another)            = delete;
    ILogLevelTableItemWidgetFactory& operator=(const ILogLevelTableItemWidgetFactory& another) = delete;

    virtual ILogLevelTableItemWidget* newInstance(QWidget* parent) const = 0;
};
