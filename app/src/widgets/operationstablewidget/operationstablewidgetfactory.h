#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"



class OperationsTableWidgetFactory : public IOperationsTableWidgetFactory
{
public:
    OperationsTableWidgetFactory();
    ~OperationsTableWidgetFactory();

    OperationsTableWidgetFactory(const OperationsTableWidgetFactory& another)            = delete;
    OperationsTableWidgetFactory& operator=(const OperationsTableWidgetFactory& another) = delete;

    IOperationsTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) override;
};
