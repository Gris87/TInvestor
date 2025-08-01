#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.h"
#include "ui_buydecision3configwidget.h"

#include <QDebug>



BuyDecision3ConfigWidget::BuyDecision3ConfigWidget(IBuyDecision3Config* config, QWidget* parent) :
    IBuyDecision3ConfigWidget(parent),
    ui(new Ui::BuyDecision3ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision3ConfigWidget";

    ui->setupUi(this);
}

BuyDecision3ConfigWidget::~BuyDecision3ConfigWidget()
{
    qDebug() << "Destroy BuyDecision3ConfigWidget";

    delete ui;
}

void BuyDecision3ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->priceFallDoubleSpinBox->setValue(mConfig->getPriceFall());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision3ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->priceFallDoubleSpinBox->setReadOnly(true);
    ui->durationSpinBox->setReadOnly(true);
}

void BuyDecision3ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->priceFallDoubleSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision3ConfigWidget::on_priceFallDoubleSpinBox_valueChanged(double value)
{
    mConfig->setPriceFall(value);
}

void BuyDecision3ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
