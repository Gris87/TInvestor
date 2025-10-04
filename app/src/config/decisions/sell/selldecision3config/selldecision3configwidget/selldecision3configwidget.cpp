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
    ui->loseYieldDoubleSpinBox->setValue(mConfig->getLoseYield());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void SellDecision3ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->loseYieldDoubleSpinBox->setReadOnly(true);
    ui->durationSpinBox->setReadOnly(true);
}

void SellDecision3ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->loseYieldDoubleSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void SellDecision3ConfigWidget::on_loseYieldDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLoseYield(value);
}

void SellDecision3ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
