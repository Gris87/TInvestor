#pragma once



#include "src/widgets/portfoliotablewidget/iportfoliotablewidget.h"

#include <gmock/gmock.h>



class PortfolioTableWidgetMock : public IPortfolioTableWidget
{
    Q_OBJECT

public:
    explicit PortfolioTableWidgetMock(QWidget* parent = nullptr) :
        IPortfolioTableWidget(parent)
    {
    }
    ~PortfolioTableWidgetMock() override = default;

    PortfolioTableWidgetMock(const PortfolioTableWidgetMock& another)            = delete;
    PortfolioTableWidgetMock& operator=(const PortfolioTableWidgetMock& another) = delete;

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
