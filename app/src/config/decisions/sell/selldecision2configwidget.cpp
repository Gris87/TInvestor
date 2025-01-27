#include "selldecision2configwidget.h"
#include "ui_selldecision2configwidget.h"

#include <QDebug>



SellDecision2ConfigWidget::SellDecision2ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SellDecision2ConfigWidget)
{
    qDebug() << "Create SellDecision2ConfigWidget";

    ui->setupUi(this);
}

SellDecision2ConfigWidget::~SellDecision2ConfigWidget()
{
    qDebug() << "Destroy SellDecision2ConfigWidget";

    delete ui;
}

void SellDecision2ConfigWidget::setSellDecision2Config(ISellDecision2Config *config)
{
    mConfig = config;
}

void SellDecision2ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->incomeAboveDoubleSpinBox->setValue(mConfig->getIncomeAbove());
    ui->loseIncomeDoubleSpinBox->setValue(mConfig->getLoseIncome());
}

void SellDecision2ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->incomeAboveDoubleSpinBox->setEnabled(checked);
    ui->loseIncomeDoubleSpinBox->setEnabled(checked);
}

void SellDecision2ConfigWidget::on_incomeAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setIncomeAbove(value);
}

void SellDecision2ConfigWidget::on_loseIncomeDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLoseIncome(value);
}
