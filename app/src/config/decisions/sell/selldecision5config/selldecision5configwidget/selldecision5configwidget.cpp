#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/selldecision5configwidget.h"
#include "ui_selldecision5configwidget.h"

#include <QDebug>



SellDecision5ConfigWidget::SellDecision5ConfigWidget(ISellDecision5Config* config, QWidget* parent) :
    ISellDecision5ConfigWidget(parent),
    ui(new Ui::SellDecision5ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create SellDecision5ConfigWidget";

    ui->setupUi(this);
}

SellDecision5ConfigWidget::~SellDecision5ConfigWidget()
{
    qDebug() << "Destroy SellDecision5ConfigWidget";

    delete ui;
}

void SellDecision5ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->durationSpinBox->setValue(mConfig->getDuration());
    ui->yieldAboveDoubleSpinBox->setValue(mConfig->getYieldAbove());
}

void SellDecision5ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->durationSpinBox->setReadOnly(true);
    ui->yieldAboveDoubleSpinBox->setReadOnly(true);
}

void SellDecision5ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->durationSpinBox->setEnabled(checked);
    ui->yieldAboveDoubleSpinBox->setEnabled(checked);
}

void SellDecision5ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}

void SellDecision5ConfigWidget::on_yieldAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setYieldAbove(value);
}
