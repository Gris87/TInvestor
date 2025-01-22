#include "buydecision1configwidget.h"
#include "ui_buydecision1configwidget.h"

#include <QDebug>



BuyDecision1ConfigWidget::BuyDecision1ConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuyDecision1ConfigWidget)
{
    qDebug() << "Create BuyDecision1ConfigWidget";

    ui->setupUi(this);
}

BuyDecision1ConfigWidget::~BuyDecision1ConfigWidget()
{
    qDebug() << "Destroy BuyDecision1ConfigWidget";

    delete ui;
}

void BuyDecision1ConfigWidget::setBuyDecision1Config(BuyDecision1Config *config)
{
    mConfig = config;
}

void BuyDecision1ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->priceFallDoubleSpinBox->setValue(mConfig->getPriceFall());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision1ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

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
