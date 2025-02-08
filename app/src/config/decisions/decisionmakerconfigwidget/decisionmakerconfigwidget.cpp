#include "decisionmakerconfigwidget.h"
#include "ui_decisionmakerconfigwidget.h"

#include <QDebug>



DecisionMakerConfigWidget::DecisionMakerConfigWidget(
    IDecisionMakerConfig*              config,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    QWidget*                           parent
) :
    IDecisionMakerConfigWidget(parent),
    ui(new Ui::DecisionMakerConfigWidget)
{
    qDebug() << "Create DecisionMakerConfigWidget";

    ui->setupUi(this);

    // clang-format off
    mBuyDecision1ConfigWidget  = buyDecision1ConfigWidgetFactory->newInstance(config->getBuyDecision1Config(),   ui->buyGroupBox);
    mBuyDecision2ConfigWidget  = buyDecision2ConfigWidgetFactory->newInstance(config->getBuyDecision2Config(),   ui->buyGroupBox);
    mBuyDecision3ConfigWidget  = buyDecision3ConfigWidgetFactory->newInstance(config->getBuyDecision3Config(),   ui->buyGroupBox);
    mSellDecision1ConfigWidget = sellDecision1ConfigWidgetFactory->newInstance(config->getSellDecision1Config(), ui->sellGroupBox);
    mSellDecision2ConfigWidget = sellDecision2ConfigWidgetFactory->newInstance(config->getSellDecision2Config(), ui->sellGroupBox);
    mSellDecision3ConfigWidget = sellDecision3ConfigWidgetFactory->newInstance(config->getSellDecision3Config(), ui->sellGroupBox);
    // clang-format on

    ui->layoutForBuyDecisions->addWidget(mBuyDecision1ConfigWidget);
    ui->layoutForBuyDecisions->addWidget(mBuyDecision2ConfigWidget);
    ui->layoutForBuyDecisions->addWidget(mBuyDecision3ConfigWidget);
    ui->layoutForSellDecisions->addWidget(mSellDecision1ConfigWidget);
    ui->layoutForSellDecisions->addWidget(mSellDecision2ConfigWidget);
    ui->layoutForSellDecisions->addWidget(mSellDecision3ConfigWidget);
}

DecisionMakerConfigWidget::~DecisionMakerConfigWidget()
{
    qDebug() << "Destroy DecisionMakerConfigWidget";

    delete ui;
}

void DecisionMakerConfigWidget::updateUiFromConfig()
{
    mBuyDecision1ConfigWidget->updateUiFromConfig();
    mBuyDecision2ConfigWidget->updateUiFromConfig();
    mBuyDecision3ConfigWidget->updateUiFromConfig();
    mSellDecision1ConfigWidget->updateUiFromConfig();
    mSellDecision2ConfigWidget->updateUiFromConfig();
    mSellDecision3ConfigWidget->updateUiFromConfig();
}
