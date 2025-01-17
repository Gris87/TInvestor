#include "decisionmakerconfigwidget.h"
#include "ui_decisionmakerconfigwidget.h"



DecisionMakerConfigWidget::DecisionMakerConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionMakerConfigWidget)
{
    ui->setupUi(this);
}

DecisionMakerConfigWidget::~DecisionMakerConfigWidget()
{
    delete ui;
}
