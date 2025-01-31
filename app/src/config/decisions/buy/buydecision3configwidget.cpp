#include "buydecision3configwidget.h"
#include "ui_buydecision3configwidget.h"

#include <QDebug>



BuyDecision3ConfigWidget::BuyDecision3ConfigWidget(IBuyDecision3Config *config, QWidget *parent) :
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

void BuyDecision3ConfigWidget::updateUiFromConfig()
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->priceRiseDoubleSpinBox->setValue(mConfig->getPriceRise());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision3ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->priceRiseDoubleSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision3ConfigWidget::on_priceRiseDoubleSpinBox_valueChanged(double value)
{
    mConfig->setPriceRise(value);
}

void BuyDecision3ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
