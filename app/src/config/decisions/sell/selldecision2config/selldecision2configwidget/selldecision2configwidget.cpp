#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.h"
#include "ui_selldecision2configwidget.h"

#include <QDebug>



SellDecision2ConfigWidget::SellDecision2ConfigWidget(ISellDecision2Config* config, QWidget* parent) :
    ISellDecision2ConfigWidget(parent),
    ui(new Ui::SellDecision2ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create SellDecision2ConfigWidget";

    ui->setupUi(this);
}

SellDecision2ConfigWidget::~SellDecision2ConfigWidget()
{
    qDebug() << "Destroy SellDecision2ConfigWidget";

    delete ui;
}

void SellDecision2ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->yieldAboveDoubleSpinBox->setValue(mConfig->getYieldAbove());
    ui->loseYieldDoubleSpinBox->setValue(mConfig->getLoseYield());
}

void SellDecision2ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->yieldAboveDoubleSpinBox->setReadOnly(true);
    ui->loseYieldDoubleSpinBox->setReadOnly(true);
}

void SellDecision2ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->yieldAboveDoubleSpinBox->setEnabled(checked);
    ui->loseYieldDoubleSpinBox->setEnabled(checked);
}

void SellDecision2ConfigWidget::on_yieldAboveDoubleSpinBox_valueChanged(double value)
{
    mConfig->setYieldAbove(value);
}

void SellDecision2ConfigWidget::on_loseYieldDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLoseYield(value);
}
