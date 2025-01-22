#include "selldecision1configwidget.h"
#include "ui_selldecision1configwidget.h"

#include <QDebug>



SellDecision1ConfigWidget::SellDecision1ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SellDecision1ConfigWidget)
{
    qDebug() << "Create SellDecision1ConfigWidget";

    ui->setupUi(this);
}

SellDecision1ConfigWidget::~SellDecision1ConfigWidget()
{
    qDebug() << "Destroy SellDecision1ConfigWidget";

    delete ui;
}

void SellDecision1ConfigWidget::setSellDecision1Config(SellDecision1Config *config)
{
    mConfig = config;
}

void SellDecision1ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->incomeAboveDoubleSpinBox->setValue(mConfig->getIncomeAbove());
}

void SellDecision1ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->incomeAboveDoubleSpinBox->setEnabled(checked);
}

void SellDecision1ConfigWidget::on_incomeAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setIncomeAbove(value);
}

