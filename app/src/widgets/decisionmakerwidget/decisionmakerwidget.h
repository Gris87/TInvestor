#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class DecisionMakerWidget;
}



class DecisionMakerWidget : public IDecisionMakerWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerWidget(ISettingsEditor* settingsEditor, const QString& type, QWidget* parent = nullptr);
    ~DecisionMakerWidget();

    DecisionMakerWidget(const DecisionMakerWidget& another)            = delete;
    DecisionMakerWidget& operator=(const DecisionMakerWidget& another) = delete;

    void saveWindowState() override;
    void loadWindowState() override;

    Ui::DecisionMakerWidget* ui;

private:
    ISettingsEditor* mSettingsEditor;
    QString          mType;
};
