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
    ui->rsiMonthDoubleSpinBox->setValue(mConfig->getRsiMonth());
    ui->rsiWeekDoubleSpinBox->setValue(mConfig->getRsiWeek());
    ui->rsiDayDoubleSpinBox->setValue(mConfig->getRsiDay());
}

void BuyDecision9ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->rsiMonthDoubleSpinBox->setReadOnly(true);
    ui->rsiWeekDoubleSpinBox->setReadOnly(true);
    ui->rsiDayDoubleSpinBox->setReadOnly(true);
}

void BuyDecision9ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->rsiMonthDoubleSpinBox->setEnabled(checked);
    ui->rsiWeekDoubleSpinBox->setEnabled(checked);
    ui->rsiDayDoubleSpinBox->setEnabled(checked);
}

void BuyDecision9ConfigWidget::on_rsiMonthDoubleSpinBox_valueChanged(double value)
{
    mConfig->setRsiMonth(value);
}

void BuyDecision9ConfigWidget::on_rsiWeekDoubleSpinBox_valueChanged(double value)
{
    mConfig->setRsiWeek(value);
}

void BuyDecision9ConfigWidget::on_rsiDayDoubleSpinBox_valueChanged(double value)
{
    mConfig->setRsiDay(value);
}
