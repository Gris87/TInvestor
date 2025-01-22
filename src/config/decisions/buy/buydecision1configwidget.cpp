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
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->incomeAboveDoubleSpinBox->setValue(mConfig->getIncomeAbove());
}

void BuyDecision1ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->incomeAboveDoubleSpinBox->setEnabled(checked);
}

void BuyDecision1ConfigWidget::on_incomeAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setIncomeAbove(value);
}

