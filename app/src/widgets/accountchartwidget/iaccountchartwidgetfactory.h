#pragma once



#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidget.h"



class IAccountChartWidgetFactory
{
public:
    IAccountChartWidgetFactory()          = default;
    virtual ~IAccountChartWidgetFactory() = default;

    IAccountChartWidgetFactory(const IAccountChartWidgetFactory& another)            = delete;
    IAccountChartWidgetFactory& operator=(const IAccountChartWidgetFactory& another) = delete;

    virtual IAccountChartWidget*
    newInstance(IFileDialogFactory* fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent) const = 0;
};
