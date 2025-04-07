#pragma once



#include "src/widgets/tableitems/actions/iactionstableitemwidget.h"

#include <gmock/gmock.h>



class ActionsTableItemWidgetMock : public IActionsTableItemWidget
{
    Q_OBJECT

public:
    explicit ActionsTableItemWidgetMock(QWidget* parent = nullptr) :
        IActionsTableItemWidget(parent)
    {
    }
    ~ActionsTableItemWidgetMock() override = default;

    ActionsTableItemWidgetMock(const ActionsTableItemWidgetMock& another)            = delete;
    ActionsTableItemWidgetMock& operator=(const ActionsTableItemWidgetMock& another) = delete;
};
