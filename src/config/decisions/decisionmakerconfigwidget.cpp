#include "decisionmakerconfigwidget.h"
#include "ui_decisionmakerconfigwidget.h"

#include <QDebug>



DecisionMakerConfigWidget::DecisionMakerConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionMakerConfigWidget)
{
    qDebug() << "Create DecisionMakerConfigWidget";

    ui->setupUi(this);
}

DecisionMakerConfigWidget::~DecisionMakerConfigWidget()
{
    qDebug() << "Destroy DecisionMakerConfigWidget";

    delete ui;
}

void DecisionMakerConfigWidget::setDecisionMakerConfig(DecisionMakerConfig *config)
{
    mConfig = config;
}

void DecisionMakerConfigWidget::updateUiFromConfig()
{

}
