#ifndef DECISIONMAKERCONFIGWIDGET_H
#define DECISIONMAKERCONFIGWIDGET_H

#include <QWidget>

#include "src/config/decisionmakerconfig.h"



namespace Ui {
class DecisionMakerConfigWidget;
}



class DecisionMakerConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerConfigWidget(QWidget *parent = nullptr);
    ~DecisionMakerConfigWidget();

    void setDecisionMakerConfig(DecisionMakerConfig *config);
    void updateUiFromConfig();

private:
    Ui::DecisionMakerConfigWidget *ui;

    DecisionMakerConfig *mConfig;
};



#endif // DECISIONMAKERCONFIGWIDGET_H
