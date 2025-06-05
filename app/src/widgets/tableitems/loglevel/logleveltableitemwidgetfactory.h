#pragma once



#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory.h"



class LogLevelTableItemWidgetFactory : public ILogLevelTableItemWidgetFactory
{
public:
    LogLevelTableItemWidgetFactory();
    ~LogLevelTableItemWidgetFactory() override;

    LogLevelTableItemWidgetFactory(const LogLevelTableItemWidgetFactory& another)            = delete;
    LogLevelTableItemWidgetFactory& operator=(const LogLevelTableItemWidgetFactory& another) = delete;

    ILogLevelTableItemWidget* newInstance(QWidget* parent) const override;
};
