#include "src/widgets/filterwidget/filterwidget.h"
#include "ui_filterwidget.h"

#include <QDebug>



FilterWidget::FilterWidget(QWidget* parent) :
    IFilterWidget(parent),
    ui(new Ui::FilterWidget),
    mFilter()
{
    qDebug() << "Create FilterWidget";

    ui->setupUi(this);
}

FilterWidget::~FilterWidget()
{
    qDebug() << "Destroy FilterWidget";

    delete ui;
}

void FilterWidget::on_tickerCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mFilter.useTicker = checked;
    ui->tickerLineEdit->setEnabled(checked);

    emit filterChanged(mFilter);
}

void FilterWidget::on_tickerLineEdit_editingFinished()
{
    mFilter.ticker = ui->tickerLineEdit->text();

    emit filterChanged(mFilter);
}

void FilterWidget::on_priceCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mFilter.usePrice = checked;
    ui->priceFromDoubleSpinBox->setEnabled(checked);
    ui->priceToDoubleSpinBox->setEnabled(checked);

    emit filterChanged(mFilter);
}

void FilterWidget::on_priceFromDoubleSpinBox_editingFinished()
{
    mFilter.priceFrom = ui->priceFromDoubleSpinBox->value();

    if (mFilter.priceFrom > mFilter.priceTo)
    {
        mFilter.priceTo = mFilter.priceFrom;
        ui->priceToDoubleSpinBox->setValue(mFilter.priceTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_priceToDoubleSpinBox_editingFinished()
{
    mFilter.priceTo = ui->priceToDoubleSpinBox->value();

    if (mFilter.priceTo < mFilter.priceFrom)
    {
        mFilter.priceFrom = mFilter.priceTo;
        ui->priceFromDoubleSpinBox->setValue(mFilter.priceFrom);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_dayStartChangeCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mFilter.useDayStartChange = checked;
    ui->dayStartChangeFromDoubleSpinBox->setEnabled(checked);
    ui->dayStartChangeToDoubleSpinBox->setEnabled(checked);

    emit filterChanged(mFilter);
}

void FilterWidget::on_dayStartChangeFromDoubleSpinBox_editingFinished()
{
    mFilter.dayStartChangeFrom = ui->dayStartChangeFromDoubleSpinBox->value();

    if (mFilter.dayStartChangeFrom > mFilter.dayStartChangeTo)
    {
        mFilter.dayStartChangeTo = mFilter.dayStartChangeFrom;
        ui->dayStartChangeToDoubleSpinBox->setValue(mFilter.dayStartChangeTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_dayStartChangeToDoubleSpinBox_editingFinished()
{
    mFilter.dayStartChangeTo = ui->dayStartChangeToDoubleSpinBox->value();

    if (mFilter.dayStartChangeTo < mFilter.dayStartChangeFrom)
    {
        mFilter.dayStartChangeFrom = mFilter.dayStartChangeTo;
        ui->dayStartChangeFromDoubleSpinBox->setValue(mFilter.dayStartChangeFrom);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_dateChangeCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mFilter.useDateChange = checked;
    ui->dateChangeFromDoubleSpinBox->setEnabled(checked);
    ui->dateChangeToDoubleSpinBox->setEnabled(checked);

    emit filterChanged(mFilter);
}

void FilterWidget::on_dateChangeFromDoubleSpinBox_editingFinished()
{
    mFilter.dateChangeFrom = ui->dateChangeFromDoubleSpinBox->value();

    if (mFilter.dateChangeFrom > mFilter.dateChangeTo)
    {
        mFilter.dateChangeTo = mFilter.dateChangeFrom;
        ui->dateChangeToDoubleSpinBox->setValue(mFilter.dateChangeTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_dateChangeToDoubleSpinBox_editingFinished()
{
    mFilter.dateChangeTo = ui->dateChangeToDoubleSpinBox->value();

    if (mFilter.dateChangeTo < mFilter.dateChangeFrom)
    {
        mFilter.dateChangeFrom = mFilter.dateChangeTo;
        ui->dateChangeFromDoubleSpinBox->setValue(mFilter.dateChangeFrom);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_paybackCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mFilter.usePayback = checked;
    ui->paybackFromDoubleSpinBox->setEnabled(checked);
    ui->paybackToDoubleSpinBox->setEnabled(checked);

    emit filterChanged(mFilter);
}

void FilterWidget::on_paybackFromDoubleSpinBox_editingFinished()
{
    mFilter.paybackFrom = ui->paybackFromDoubleSpinBox->value();

    if (mFilter.paybackFrom > mFilter.paybackTo)
    {
        mFilter.paybackTo = mFilter.paybackFrom;
        ui->paybackToDoubleSpinBox->setValue(mFilter.paybackTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_paybackToDoubleSpinBox_editingFinished()
{
    mFilter.paybackTo = ui->paybackToDoubleSpinBox->value();

    if (mFilter.paybackTo < mFilter.paybackFrom)
    {
        mFilter.paybackFrom = mFilter.paybackTo;
        ui->paybackFromDoubleSpinBox->setValue(mFilter.paybackFrom);
    }

    emit filterChanged(mFilter);
}
