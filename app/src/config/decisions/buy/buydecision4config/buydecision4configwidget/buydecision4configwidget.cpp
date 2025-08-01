#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/buydecision4configwidget.h"
#include "ui_buydecision4configwidget.h"

#include <QDebug>



BuyDecision4ConfigWidget::BuyDecision4ConfigWidget(IBuyDecision4Config* config, QWidget* parent) :
    IBuyDecision4ConfigWidget(parent),
    ui(new Ui::BuyDecision4ConfigWidget),
    mConfig(config)
{
    qDebug() << "Create BuyDecision4ConfigWidget";

    ui->setupUi(this);
}

BuyDecision4ConfigWidget::~BuyDecision4ConfigWidget()
{
    qDebug() << "Destroy BuyDecision4ConfigWidget";

    delete ui;
}

void BuyDecision4ConfigWidget::updateUiFromConfig() const
{
    ui->enabledCheckBox->setChecked(mConfig->isEnabled());
    ui->priceFallDoubleSpinBox->setValue(mConfig->getPriceFall());
    ui->loseYieldDoubleSpinBox->setValue(mConfig->getLoseYield());
    ui->durationSpinBox->setValue(mConfig->getDuration());
}

void BuyDecision4ConfigWidget::makeReadOnly() const
{
    ui->enabledCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->enabledCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->priceFallDoubleSpinBox->setReadOnly(true);
    ui->loseYieldDoubleSpinBox->setReadOnly(true);
    ui->durationSpinBox->setReadOnly(true);
}

void BuyDecision4ConfigWidget::on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setEnabled(checked);

    ui->priceFallDoubleSpinBox->setEnabled(checked);
    ui->loseYieldDoubleSpinBox->setEnabled(checked);
    ui->durationSpinBox->setEnabled(checked);
}

void BuyDecision4ConfigWidget::on_priceFallDoubleSpinBox_valueChanged(double value)
{
    mConfig->setPriceFall(value);
}

void BuyDecision4ConfigWidget::on_loseYieldDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLoseYield(value);
}

void BuyDecision4ConfigWidget::on_durationSpinBox_valueChanged(int value)
{
    mConfig->setDuration(value);
}
