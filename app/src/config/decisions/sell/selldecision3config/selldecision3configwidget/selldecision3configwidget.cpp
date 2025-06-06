#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.h"
#include "ui_selldecision3configwidget.h"

#include <QDebug>



SellDecision3ConfigWidget::SellDecision3ConfigWidget(ISellDecision3Config* config, QWidget* parent) :
    ISellDecision3ConfigWidget(parent),
    ui(new Ui::SellDecision3ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create SellDecision3ConfigWidget";

    ui->setupUi(this);
}

SellDecision3ConfigWidget::~SellDecision3ConfigWidget()
{
    qDebug() << "Destroy SellDecision3ConfigWidget";

    delete ui;
}

void SellDecision3ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->durationSpinBox->setValue(mConfig->getDuration());
    ui->loseIncomeDoubleSpinBox->setValue(mConfig->getLoseIncome());
}

void SellDecision3ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->durationSpinBox->setEnabled(checked);
    ui->loseIncomeDoubleSpinBox->setEnabled(checked);
}

void SellDecision3ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}

void SellDecision3ConfigWidget::on_loseIncomeDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLoseIncome(value);
}
