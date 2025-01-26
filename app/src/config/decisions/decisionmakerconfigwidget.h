#pragma once



#include <QWidget>

#include "src/config/decisions/idecisionmakerconfig.h"



namespace Ui {
class DecisionMakerConfigWidget;
}



class DecisionMakerConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerConfigWidget(QWidget *parent = nullptr);
    ~DecisionMakerConfigWidget();

    void setDecisionMakerConfig(IDecisionMakerConfig *config);
    void updateUiFromConfig();

private:
    Ui::DecisionMakerConfigWidget *ui;
};
