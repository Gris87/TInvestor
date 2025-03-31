#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IDecisionMakerWidgetFactory
{
public:
    IDecisionMakerWidgetFactory()
    {
    }
    virtual ~IDecisionMakerWidgetFactory() = default;

    IDecisionMakerWidgetFactory(const IDecisionMakerWidgetFactory& another)            = delete;
    IDecisionMakerWidgetFactory& operator=(const IDecisionMakerWidgetFactory& another) = delete;

    virtual IDecisionMakerWidget* newInstance(ISettingsEditor* settingsEditor, const QString& type, QWidget* parent) = 0;
};
