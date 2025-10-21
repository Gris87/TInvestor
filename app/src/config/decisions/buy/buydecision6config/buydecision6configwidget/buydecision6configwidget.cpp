#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/buydecision6configwidget.h"
#include "ui_buydecision6configwidget.h"

#include <QDebug>



BuyDecision6ConfigWidget::BuyDecision6ConfigWidget(IBuyDecision6Config* config, QWidget* parent) :
    IBuyDecision6ConfigWidget(parent),
    ui(new Ui::BuyDecision6ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision6ConfigWidget";

    ui->setupUi(this);
}

BuyDecision6ConfigWidget::~BuyDecision6ConfigWidget()
{
    qDebug() << "Destroy BuyDecision6ConfigWidget";

    delete ui;
}

void BuyDecision6ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->rsiDoubleSpinBox->setValue(mConfig->getRsi());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision6ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->rsiDoubleSpinBox->setReadOnly(true);
    ui->durationSpinBox->setReadOnly(true);
}

void BuyDecision6ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->rsiDoubleSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision6ConfigWidget::on_rsiDoubleSpinBox_valueChanged(double value)
{
    mConfig->setRsi(value);
}

void BuyDecision6ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
