#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/buydecision8configwidget.h"
#include "ui_buydecision8configwidget.h"

#include <QDebug>



BuyDecision8ConfigWidget::BuyDecision8ConfigWidget(IBuyDecision8Config* config, QWidget* parent) :
    IBuyDecision8ConfigWidget(parent),
    ui(new Ui::BuyDecision8ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision8ConfigWidget";

    ui->setupUi(this);
}

BuyDecision8ConfigWidget::~BuyDecision8ConfigWidget()
{
    qDebug() << "Destroy BuyDecision8ConfigWidget";

    delete ui;
}

void BuyDecision8ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision8ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->durationSpinBox->setReadOnly(true);
}

void BuyDecision8ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision8ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
