#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/buydecision9configwidget.h"
#include "ui_buydecision9configwidget.h"

#include <QDebug>



BuyDecision9ConfigWidget::BuyDecision9ConfigWidget(IBuyDecision9Config* config, QWidget* parent) :
    IBuyDecision9ConfigWidget(parent),
    ui(new Ui::BuyDecision9ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision9ConfigWidget";

    ui->setupUi(this);
}

BuyDecision9ConfigWidget::~BuyDecision9ConfigWidget()
{
    qDebug() << "Destroy BuyDecision9ConfigWidget";

    delete ui;
}

void BuyDecision9ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->rsiDoubleSpinBox->setValue(mConfig->getRsi());
}

void BuyDecision9ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->rsiDoubleSpinBox->setReadOnly(true);
}

void BuyDecision9ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->rsiDoubleSpinBox->setEnabled(checked);
}

void BuyDecision9ConfigWidget::on_rsiDoubleSpinBox_valueChanged(double value)
{
    mConfig->setRsi(value);
}
