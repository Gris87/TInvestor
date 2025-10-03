#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.h"
#include "ui_decisionmakerconfigwidget.h"

#include <QDebug>



DecisionMakerConfigWidget::DecisionMakerConfigWidget(
    IDecisionMakerConfig*              config,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory,
    IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory,
    IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory,
    IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory,
    IBuyDecision8ConfigWidgetFactory*  buyDecision8ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory,
    ISellDecision5ConfigWidgetFactory* sellDecision5ConfigWidgetFactory,
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
    mBuyDecision4ConfigWidget  = buyDecision4ConfigWidgetFactory->newInstance(config->getBuyDecision4Config(),   ui->buyGroupBox);
    mBuyDecision5ConfigWidget  = buyDecision5ConfigWidgetFactory->newInstance(config->getBuyDecision5Config(),   ui->buyGroupBox);
    mBuyDecision6ConfigWidget  = buyDecision6ConfigWidgetFactory->newInstance(config->getBuyDecision6Config(),   ui->buyGroupBox);
    mBuyDecision7ConfigWidget  = buyDecision7ConfigWidgetFactory->newInstance(config->getBuyDecision7Config(),   ui->buyGroupBox);
    mBuyDecision8ConfigWidget  = buyDecision8ConfigWidgetFactory->newInstance(config->getBuyDecision8Config(),   ui->buyGroupBox);
    mSellDecision1ConfigWidget = sellDecision1ConfigWidgetFactory->newInstance(config->getSellDecision1Config(), ui->sellGroupBox);
    mSellDecision2ConfigWidget = sellDecision2ConfigWidgetFactory->newInstance(config->getSellDecision2Config(), ui->sellGroupBox);
    mSellDecision3ConfigWidget = sellDecision3ConfigWidgetFactory->newInstance(config->getSellDecision3Config(), ui->sellGroupBox);
    mSellDecision4ConfigWidget = sellDecision4ConfigWidgetFactory->newInstance(config->getSellDecision4Config(), ui->sellGroupBox);
    mSellDecision5ConfigWidget = sellDecision5ConfigWidgetFactory->newInstance(config->getSellDecision5Config(), ui->sellGroupBox);
    // clang-format on

    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision1ConfigWidget);
    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision2ConfigWidget);
    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision3ConfigWidget);
    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision4ConfigWidget);
    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision5ConfigWidget);
    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision6ConfigWidget);
    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision7ConfigWidget);
    ui->layoutForBuyDecisionWidgets->addWidget(mBuyDecision8ConfigWidget);
    ui->layoutForSellDecisionWidgets->addWidget(mSellDecision1ConfigWidget);
    ui->layoutForSellDecisionWidgets->addWidget(mSellDecision2ConfigWidget);
    ui->layoutForSellDecisionWidgets->addWidget(mSellDecision3ConfigWidget);
    ui->layoutForSellDecisionWidgets->addWidget(mSellDecision4ConfigWidget);
    ui->layoutForSellDecisionWidgets->addWidget(mSellDecision5ConfigWidget);
}

DecisionMakerConfigWidget::~DecisionMakerConfigWidget()
{
    qDebug() << "Destroy DecisionMakerConfigWidget";

    delete ui;
}

void DecisionMakerConfigWidget::updateUiFromConfig() const
{
    mBuyDecision1ConfigWidget->updateUiFromConfig();
    mBuyDecision2ConfigWidget->updateUiFromConfig();
    mBuyDecision3ConfigWidget->updateUiFromConfig();
    mBuyDecision4ConfigWidget->updateUiFromConfig();
    mBuyDecision5ConfigWidget->updateUiFromConfig();
    mBuyDecision6ConfigWidget->updateUiFromConfig();
    mBuyDecision7ConfigWidget->updateUiFromConfig();
    mBuyDecision8ConfigWidget->updateUiFromConfig();
    mSellDecision1ConfigWidget->updateUiFromConfig();
    mSellDecision2ConfigWidget->updateUiFromConfig();
    mSellDecision3ConfigWidget->updateUiFromConfig();
    mSellDecision4ConfigWidget->updateUiFromConfig();
    mSellDecision5ConfigWidget->updateUiFromConfig();
}

void DecisionMakerConfigWidget::makeReadOnly() const
{
    mBuyDecision1ConfigWidget->makeReadOnly();
    mBuyDecision2ConfigWidget->makeReadOnly();
    mBuyDecision3ConfigWidget->makeReadOnly();
    mBuyDecision4ConfigWidget->makeReadOnly();
    mBuyDecision5ConfigWidget->makeReadOnly();
    mBuyDecision6ConfigWidget->makeReadOnly();
    mBuyDecision7ConfigWidget->makeReadOnly();
    mBuyDecision8ConfigWidget->makeReadOnly();
    mSellDecision1ConfigWidget->makeReadOnly();
    mSellDecision2ConfigWidget->makeReadOnly();
    mSellDecision3ConfigWidget->makeReadOnly();
    mSellDecision4ConfigWidget->makeReadOnly();
    mSellDecision5ConfigWidget->makeReadOnly();
}
