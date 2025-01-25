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
    ui->buyDecision1ConfigWidget->setBuyDecision1Config(&config->buyDecision1Config);
    ui->buyDecision2ConfigWidget->setBuyDecision2Config(&config->buyDecision2Config);
    ui->buyDecision3ConfigWidget->setBuyDecision3Config(&config->buyDecision3Config);
    ui->sellDecision1ConfigWidget->setSellDecision1Config(&config->sellDecision1Config);
    ui->sellDecision2ConfigWidget->setSellDecision2Config(&config->sellDecision2Config);
    ui->sellDecision3ConfigWidget->setSellDecision3Config(&config->sellDecision3Config);
}

void DecisionMakerConfigWidget::updateUiFromConfig()
{
    ui->buyDecision1ConfigWidget->updateUiFromConfig();
    ui->buyDecision2ConfigWidget->updateUiFromConfig();
    ui->buyDecision3ConfigWidget->updateUiFromConfig();
    ui->sellDecision1ConfigWidget->updateUiFromConfig();
    ui->sellDecision2ConfigWidget->updateUiFromConfig();
    ui->sellDecision3ConfigWidget->updateUiFromConfig();
}
