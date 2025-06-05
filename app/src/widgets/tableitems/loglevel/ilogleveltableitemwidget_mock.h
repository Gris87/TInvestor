#pragma once



#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidget.h"

#include <gmock/gmock.h>



class LogLevelTableItemWidgetMock : public ILogLevelTableItemWidget
{
    Q_OBJECT

public:
    explicit LogLevelTableItemWidgetMock(QWidget* parent = nullptr) :
        ILogLevelTableItemWidget(parent)
    {
    }
    ~LogLevelTableItemWidgetMock() override = default;

    LogLevelTableItemWidgetMock(const LogLevelTableItemWidgetMock& another)            = delete;
    LogLevelTableItemWidgetMock& operator=(const LogLevelTableItemWidgetMock& another) = delete;
};
