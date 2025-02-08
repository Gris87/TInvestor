#include "buydecision2configwidget.h"
#include "ui_buydecision2configwidget.h"

#include <QDebug>



BuyDecision2ConfigWidget::BuyDecision2ConfigWidget(IBuyDecision2Config* config, QWidget* parent) :
    IBuyDecision2ConfigWidget(parent),
    ui(new Ui::BuyDecision2ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision2ConfigWidget";

    ui->setupUi(this);
}

BuyDecision2ConfigWidget::~BuyDecision2ConfigWidget()
{
    qDebug() << "Destroy BuyDecision2ConfigWidget";

    delete ui;
}

void BuyDecision2ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->priceDiffDoubleSpinBox->setValue(mConfig->getPriceDiff());
    ui->amountOfTimesSpinBox->setValue(mConfig->getAmountOfTimes());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision2ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->priceDiffDoubleSpinBox->setEnabled(checked);
    ui->amountOfTimesSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision2ConfigWidget::on_priceDiffDoubleSpinBox_valueChanged(double value)
{
    mConfig->setPriceDiff(value);
}

void BuyDecision2ConfigWidget::on_amountOfTimesSpinBox_valueChanged(int value)
{
    mConfig->setAmountOfTimes(value);
}

void BuyDecision2ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
