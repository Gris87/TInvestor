#include "buydecision1configwidget.h"
#include "ui_buydecision1configwidget.h"

#include <QDebug>



BuyDecision1ConfigWidget::BuyDecision1ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuyDecision1ConfigWidget)
{
    qDebug() << "Create BuyDecision1ConfigWidget";

    ui->setupUi(this);
}

BuyDecision1ConfigWidget::~BuyDecision1ConfigWidget()
{
    qDebug() << "Destroy BuyDecision1ConfigWidget";

    delete ui;
}

void BuyDecision1ConfigWidget::setBuyDecision1Config(BuyDecision1Config *config)
{
    mConfig = config;
}

void BuyDecision1ConfigWidget::updateUiFromConfig()
{

}
