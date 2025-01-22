#include "selldecision3configwidget.h"
#include "ui_selldecision3configwidget.h"

#include <QDebug>



SellDecision3ConfigWidget::SellDecision3ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SellDecision3ConfigWidget)
{
    qDebug() << "Create SellDecision3ConfigWidget";

    ui->setupUi(this);
}

SellDecision3ConfigWidget::~SellDecision3ConfigWidget()
{
    qDebug() << "Destroy SellDecision3ConfigWidget";

    delete ui;
}

void SellDecision3ConfigWidget::setSellDecision3Config(SellDecision3Config *config)
{
    mConfig = config;
}

void SellDecision3ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->incomeAboveDoubleSpinBox->setValue(mConfig->getIncomeAbove());
}

void SellDecision3ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->incomeAboveDoubleSpinBox->setEnabled(checked);
}

void SellDecision3ConfigWidget::on_incomeAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setIncomeAbove(value);
}

