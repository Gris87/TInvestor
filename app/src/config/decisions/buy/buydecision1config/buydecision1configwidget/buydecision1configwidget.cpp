#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.h"
#include "ui_buydecision1configwidget.h"

#include <QDebug>



BuyDecision1ConfigWidget::BuyDecision1ConfigWidget(IBuyDecision1Config* config, QWidget* parent) :
    IBuyDecision1ConfigWidget(parent),
    ui(new Ui::BuyDecision1ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision1ConfigWidget";

    ui->setupUi(this);
}

BuyDecision1ConfigWidget::~BuyDecision1ConfigWidget()
{
    qDebug() << "Destroy BuyDecision1ConfigWidget";

    delete ui;
}

void BuyDecision1ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->priceFallDoubleSpinBox->setValue(mConfig->getPriceFall());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision1ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->priceFallDoubleSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision1ConfigWidget::on_priceFallDoubleSpinBox_valueChanged(double value)
{
    mConfig->setPriceFall(value);
}

void BuyDecision1ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
