#include "buydecision3configwidget.h"
#include "ui_buydecision3configwidget.h"

#include <QDebug>



BuyDecision3ConfigWidget::BuyDecision3ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuyDecision3ConfigWidget)
{
    qDebug() << "Create BuyDecision3ConfigWidget";

    ui->setupUi(this);
}

BuyDecision3ConfigWidget::~BuyDecision3ConfigWidget()
{
    qDebug() << "Destroy BuyDecision3ConfigWidget";

    delete ui;
}

void BuyDecision3ConfigWidget::setBuyDecision3Config(BuyDecision3Config *config)
{
    mConfig = config;
}

void BuyDecision3ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->incomeAboveDoubleSpinBox->setValue(mConfig->getIncomeAbove());
}

void BuyDecision3ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->incomeAboveDoubleSpinBox->setEnabled(checked);
}

void BuyDecision3ConfigWidget::on_incomeAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setIncomeAbove(value);
}

