#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidget.h"

#include <gmock/gmock.h>



class OperationsTableWidgetMock : public IOperationsTableWidget
{
    Q_OBJECT

public:
    explicit OperationsTableWidgetMock(QWidget* parent = nullptr) :
        IOperationsTableWidget(parent)
    {
    }
    ~OperationsTableWidgetMock() override = default;

    OperationsTableWidgetMock(const OperationsTableWidgetMock& another)            = delete;
    OperationsTableWidgetMock& operator=(const OperationsTableWidgetMock& another) = delete;

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
