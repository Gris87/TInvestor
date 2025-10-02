#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/buydecision7configwidget.h"
#include "ui_buydecision7configwidget.h"

#include <QDebug>



BuyDecision7ConfigWidget::BuyDecision7ConfigWidget(IBuyDecision7Config* config, QWidget* parent) :
    IBuyDecision7ConfigWidget(parent),
    ui(new Ui::BuyDecision7ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision7ConfigWidget";

    ui->setupUi(this);
}

BuyDecision7ConfigWidget::~BuyDecision7ConfigWidget()
{
    qDebug() << "Destroy BuyDecision7ConfigWidget";

    delete ui;
}

void BuyDecision7ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->priceRaiseDoubleSpinBox->setValue(mConfig->getPriceRaise());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision7ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->priceRaiseDoubleSpinBox->setReadOnly(true);
    ui->durationSpinBox->setReadOnly(true);
}

void BuyDecision7ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->priceRaiseDoubleSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision7ConfigWidget::on_priceRaiseDoubleSpinBox_valueChanged(double value)
{
    mConfig->setPriceRaise(value);
}

void BuyDecision7ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
