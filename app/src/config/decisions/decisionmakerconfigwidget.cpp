#include "decisionmakerconfigwidget.h"
#include "ui_decisionmakerconfigwidget.h"

#include <QDebug>

#include "src/config/decisions/decisionmakerconfig.h"



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

void DecisionMakerConfigWidget::setDecisionMakerConfig(IDecisionMakerConfig *config)
{
    DecisionMakerConfig *cfg = dynamic_cast<DecisionMakerConfig *>(config);

    ui->buyDecision1ConfigWidget->setBuyDecision1Config(&cfg->buyDecision1Config);
    ui->buyDecision2ConfigWidget->setBuyDecision2Config(&cfg->buyDecision2Config);
    ui->buyDecision3ConfigWidget->setBuyDecision3Config(&cfg->buyDecision3Config);
    ui->sellDecision1ConfigWidget->setSellDecision1Config(&cfg->sellDecision1Config);
    ui->sellDecision2ConfigWidget->setSellDecision2Config(&cfg->sellDecision2Config);
    ui->sellDecision3ConfigWidget->setSellDecision3Config(&cfg->sellDecision3Config);
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
