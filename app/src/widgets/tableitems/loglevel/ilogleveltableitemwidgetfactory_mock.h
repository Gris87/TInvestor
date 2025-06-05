#pragma once



#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory.h"

#include <gmock/gmock.h>



class LogLevelTableItemWidgetFactoryMock : public ILogLevelTableItemWidgetFactory
{
public:
    LogLevelTableItemWidgetFactoryMock() :
        ILogLevelTableItemWidgetFactory()
    {
    }
    ~LogLevelTableItemWidgetFactoryMock() override = default;

    LogLevelTableItemWidgetFactoryMock(const LogLevelTableItemWidgetFactoryMock& another)            = delete;
    LogLevelTableItemWidgetFactoryMock& operator=(const LogLevelTableItemWidgetFactoryMock& another) = delete;

    MOCK_METHOD(ILogLevelTableItemWidget*, newInstance, (QWidget * parent), (const, override));
};
