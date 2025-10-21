#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/buydecision5configwidget.h"
#include "ui_buydecision5configwidget.h"

#include <QDebug>



BuyDecision5ConfigWidget::BuyDecision5ConfigWidget(IBuyDecision5Config* config, QWidget* parent) :
    IBuyDecision5ConfigWidget(parent),
    ui(new Ui::BuyDecision5ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision5ConfigWidget";

    ui->setupUi(this);
}

BuyDecision5ConfigWidget::~BuyDecision5ConfigWidget()
{
    qDebug() << "Destroy BuyDecision5ConfigWidget";

    delete ui;
}

void BuyDecision5ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision5ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->durationSpinBox->setReadOnly(true);
}

void BuyDecision5ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision5ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
