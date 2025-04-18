#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IOperationsTableWidgetFactory
{
public:
    IOperationsTableWidgetFactory()          = default;
    virtual ~IOperationsTableWidgetFactory() = default;

    IOperationsTableWidgetFactory(const IOperationsTableWidgetFactory& another)            = delete;
    IOperationsTableWidgetFactory& operator=(const IOperationsTableWidgetFactory& another) = delete;

    virtual IOperationsTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const = 0;
};
