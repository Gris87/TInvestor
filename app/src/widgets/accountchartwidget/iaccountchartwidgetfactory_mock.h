#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"

#include <gmock/gmock.h>



class AccountChartWidgetFactoryMock : public IAccountChartWidgetFactory
{
public:
    AccountChartWidgetFactoryMock() :
        IAccountChartWidgetFactory()
    {
    }
    ~AccountChartWidgetFactoryMock() override = default;

    AccountChartWidgetFactoryMock(const AccountChartWidgetFactoryMock& another)            = delete;
    AccountChartWidgetFactoryMock& operator=(const AccountChartWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IAccountChartWidget*,
        newInstance,
        (IFileDialogFactory * fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent),
        (const, override)
    );
};
