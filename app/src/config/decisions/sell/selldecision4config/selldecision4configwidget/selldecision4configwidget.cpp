#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/selldecision4configwidget.h"
#include "ui_selldecision4configwidget.h"

#include <QDebug>



SellDecision4ConfigWidget::SellDecision4ConfigWidget(ISellDecision4Config* config, QWidget* parent) :
    ISellDecision4ConfigWidget(parent),
    ui(new Ui::SellDecision4ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create SellDecision4ConfigWidget";

    ui->setupUi(this);
}

SellDecision4ConfigWidget::~SellDecision4ConfigWidget()
{
    qDebug() << "Destroy SellDecision4ConfigWidget";

    delete ui;
}

void SellDecision4ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->durationSpinBox->setValue(mConfig->getDuration());
    ui->yieldAboveDoubleSpinBox->setValue(mConfig->getYieldAbove());
}

void SellDecision4ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->durationSpinBox->setReadOnly(true);
    ui->yieldAboveDoubleSpinBox->setReadOnly(true);
}

void SellDecision4ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->durationSpinBox->setEnabled(checked);
    ui->yieldAboveDoubleSpinBox->setEnabled(checked);
}

void SellDecision4ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}

void SellDecision4ConfigWidget::on_yieldAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setYieldAbove(value);
}
