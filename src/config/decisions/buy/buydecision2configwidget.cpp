#include "buydecision2configwidget.h"
#include "ui_buydecision2configwidget.h"

#include <QDebug>



BuyDecision2ConfigWidget::BuyDecision2ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuyDecision2ConfigWidget)
{
    qDebug() << "Create BuyDecision2ConfigWidget";

    ui->setupUi(this);
}

BuyDecision2ConfigWidget::~BuyDecision2ConfigWidget()
{
    qDebug() << "Destroy BuyDecision2ConfigWidget";

    delete ui;
}

void BuyDecision2ConfigWidget::setBuyDecision2Config(BuyDecision2Config *config)
{
    mConfig = config;
}

void BuyDecision2ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->incomeAboveDoubleSpinBox->setValue(mConfig->getIncomeAbove());
}

void BuyDecision2ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->incomeAboveDoubleSpinBox->setEnabled(checked);
}

void BuyDecision2ConfigWidget::on_incomeAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setIncomeAbove(value);
}

