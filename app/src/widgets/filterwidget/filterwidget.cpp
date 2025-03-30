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

const Filter& FilterWidget::getFilter()
{
    return mFilter;
}

void FilterWidget::on_tickerCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mFilter.useTicker = checked;
    ui->tickerLineEdit->setEnabled(checked);

    emit filterChanged(mFilter);
}

void FilterWidget::on_tickerLineEdit_textChanged(const QString& text)
{
    mFilter.ticker = text;

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

void FilterWidget::on_priceFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.priceFrom = value;

    if (mFilter.priceFrom > mFilter.priceTo)
    {
        mFilter.priceTo = mFilter.priceFrom;
        ui->priceToDoubleSpinBox->setValue(mFilter.priceTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_priceToDoubleSpinBox_valueChanged(double value)
{
    mFilter.priceTo = value;

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

void FilterWidget::on_dayStartChangeFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.dayStartChangeFrom = value;

    if (mFilter.dayStartChangeFrom > mFilter.dayStartChangeTo)
    {
        mFilter.dayStartChangeTo = mFilter.dayStartChangeFrom;
        ui->dayStartChangeToDoubleSpinBox->setValue(mFilter.dayStartChangeTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_dayStartChangeToDoubleSpinBox_valueChanged(double value)
{
    mFilter.dayStartChangeTo = value;

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

void FilterWidget::on_dateChangeFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.dateChangeFrom = value;

    if (mFilter.dateChangeFrom > mFilter.dateChangeTo)
    {
        mFilter.dateChangeTo = mFilter.dateChangeFrom;
        ui->dateChangeToDoubleSpinBox->setValue(mFilter.dateChangeTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_dateChangeToDoubleSpinBox_valueChanged(double value)
{
    mFilter.dateChangeTo = value;

    if (mFilter.dateChangeTo < mFilter.dateChangeFrom)
    {
        mFilter.dateChangeFrom = mFilter.dateChangeTo;
        ui->dateChangeFromDoubleSpinBox->setValue(mFilter.dateChangeFrom);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_turnoverCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    bool checked = value == Qt::Checked;

    mFilter.useTurnover = checked;
    ui->turnoverFromSpinBox->setEnabled(checked);
    ui->turnoverToSpinBox->setEnabled(checked);

    emit filterChanged(mFilter);
}

void FilterWidget::on_turnoverFromSpinBox_valueChanged(int value)
{
    mFilter.turnoverFrom = qint64(value) * 1000;

    if (mFilter.turnoverFrom > mFilter.turnoverTo)
    {
        mFilter.turnoverTo = mFilter.turnoverFrom;
        ui->turnoverToSpinBox->setValue(mFilter.turnoverTo / 1000);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_turnoverToSpinBox_valueChanged(int value)
{
    mFilter.turnoverTo = qint64(value) * 1000;

    if (mFilter.turnoverTo < mFilter.turnoverFrom)
    {
        mFilter.turnoverFrom = mFilter.turnoverTo;
        ui->turnoverFromSpinBox->setValue(mFilter.turnoverFrom / 1000);
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

void FilterWidget::on_paybackFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.paybackFrom = value;

    if (mFilter.paybackFrom > mFilter.paybackTo)
    {
        mFilter.paybackTo = mFilter.paybackFrom;
        ui->paybackToDoubleSpinBox->setValue(mFilter.paybackTo);
    }

    emit filterChanged(mFilter);
}

void FilterWidget::on_paybackToDoubleSpinBox_valueChanged(double value)
{
    mFilter.paybackTo = value;

    if (mFilter.paybackTo < mFilter.paybackFrom)
    {
        mFilter.paybackFrom = mFilter.paybackTo;
        ui->paybackFromDoubleSpinBox->setValue(mFilter.paybackFrom);
    }

    emit filterChanged(mFilter);
}
