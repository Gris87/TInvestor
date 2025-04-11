#include "src/widgets/stockscontrolswidget/stockscontrolswidget.h"
#include "ui_stockscontrolswidget.h"

#include <QDebug>



const QString    DATETIME_FORMAT     = "yyyy-MM-dd hh:mm:ss";
const QColor     GREY_COLOR          = QColor("#AFC2D7");
const QColor     GREEN_COLOR         = QColor("#2BD793");
constexpr qint64 MS_IN_SECOND        = 1000LL;
constexpr qint64 DATE_CHANGE_DELAY   = MS_IN_SECOND; // 1 second
constexpr qint64 FILTER_CHANGE_DELAY = MS_IN_SECOND; // 1 second
constexpr qint64 TURNOVER_KILOS      = 1000;         // 1K rubles
constexpr int    HIDDEN_WIDTH        = 24;
constexpr int    VISIBLE_WIDTH       = 250;
constexpr int    MAXIMUM_HEIGHT      = 16777215;



StocksControlsWidget::StocksControlsWidget(IStocksStorage* stocksStorage, ISettingsEditor* settingsEditor, QWidget* parent) :
    IStocksControlsWidget(parent),
    ui(new Ui::StocksControlsWidget),
    mStocksStorage(stocksStorage),
    mSettingsEditor(settingsEditor),
    mFilter()
{
    qDebug() << "Create StocksControlsWidget";

    ui->setupUi(this);

    dateChangeDelayTimer.setInterval(DATE_CHANGE_DELAY);
    filterChangeDelayTimer.setInterval(FILTER_CHANGE_DELAY);

    // clang-format off
    connect(&dateChangeDelayTimer,   SIGNAL(timeout()), this, SLOT(dateChangeDelayTimerTicked()));
    connect(&filterChangeDelayTimer, SIGNAL(timeout()), this, SLOT(filterChangeDelayTimerTicked()));
    // clang-format on
}

StocksControlsWidget::~StocksControlsWidget()
{
    qDebug() << "Destroy StocksControlsWidget";

    delete ui;
}

QDateTime StocksControlsWidget::getDateChangeTime()
{
    return ui->dateChangeTimeEdit->dateTime();
}

const Filter& StocksControlsWidget::getFilter()
{
    return mFilter;
}

void StocksControlsWidget::dateChangeDelayTimerTicked()
{
    dateChangeDelayTimer.stop();

    const QDateTime dateTime = ui->dateChangeTimeEdit->dateTime();

    const QMutexLocker lock(mStocksStorage->getMutex());
    mStocksStorage->obtainStocksDatePrice(dateTime.toMSecsSinceEpoch());

    emit dateChangeDateTimeChanged(dateTime);
}

void StocksControlsWidget::filterChangeDelayTimerTicked()
{
    filterChangeDelayTimer.stop();

    emit filterChanged(mFilter);
}

void StocksControlsWidget::on_dateChangeTimeEdit_dateTimeChanged(const QDateTime& /*dateTime*/)
{
    dateChangeDelayTimer.start();
}

void StocksControlsWidget::on_tickerCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mFilter.useTicker = checked;
    ui->tickerLineEdit->setEnabled(checked);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_tickerLineEdit_textChanged(const QString& text)
{
    mFilter.ticker = text;

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_qualInvestorCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mFilter.useQualInvestor = checked;
    ui->qualInvestorComboBox->setEnabled(checked);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_qualInvestorComboBox_currentIndexChanged(int index)
{
    mFilter.qualInvestor = QualInvestorFilter(index);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_priceCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mFilter.usePrice = checked;
    ui->priceFromDoubleSpinBox->setEnabled(checked);
    ui->priceToDoubleSpinBox->setEnabled(checked);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_priceFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.priceFrom = value;

    if (mFilter.priceFrom > mFilter.priceTo)
    {
        mFilter.priceTo = mFilter.priceFrom;
        ui->priceToDoubleSpinBox->setValue(mFilter.priceTo);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_priceToDoubleSpinBox_valueChanged(double value)
{
    mFilter.priceTo = value;

    if (mFilter.priceTo < mFilter.priceFrom)
    {
        mFilter.priceFrom = mFilter.priceTo;
        ui->priceFromDoubleSpinBox->setValue(mFilter.priceFrom);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_dayStartChangeCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mFilter.useDayStartChange = checked;
    ui->dayStartChangeFromDoubleSpinBox->setEnabled(checked);
    ui->dayStartChangeToDoubleSpinBox->setEnabled(checked);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_dayStartChangeFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.dayStartChangeFrom = value;

    if (mFilter.dayStartChangeFrom > mFilter.dayStartChangeTo)
    {
        mFilter.dayStartChangeTo = mFilter.dayStartChangeFrom;
        ui->dayStartChangeToDoubleSpinBox->setValue(mFilter.dayStartChangeTo);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_dayStartChangeToDoubleSpinBox_valueChanged(double value)
{
    mFilter.dayStartChangeTo = value;

    if (mFilter.dayStartChangeTo < mFilter.dayStartChangeFrom)
    {
        mFilter.dayStartChangeFrom = mFilter.dayStartChangeTo;
        ui->dayStartChangeFromDoubleSpinBox->setValue(mFilter.dayStartChangeFrom);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_dateChangeCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mFilter.useDateChange = checked;
    ui->dateChangeFromDoubleSpinBox->setEnabled(checked);
    ui->dateChangeToDoubleSpinBox->setEnabled(checked);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_dateChangeFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.dateChangeFrom = value;

    if (mFilter.dateChangeFrom > mFilter.dateChangeTo)
    {
        mFilter.dateChangeTo = mFilter.dateChangeFrom;
        ui->dateChangeToDoubleSpinBox->setValue(mFilter.dateChangeTo);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_dateChangeToDoubleSpinBox_valueChanged(double value)
{
    mFilter.dateChangeTo = value;

    if (mFilter.dateChangeTo < mFilter.dateChangeFrom)
    {
        mFilter.dateChangeFrom = mFilter.dateChangeTo;
        ui->dateChangeFromDoubleSpinBox->setValue(mFilter.dateChangeFrom);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_turnoverCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mFilter.useTurnover = checked;
    ui->turnoverFromSpinBox->setEnabled(checked);
    ui->turnoverToSpinBox->setEnabled(checked);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_turnoverFromSpinBox_valueChanged(int value)
{
    mFilter.turnoverFrom = static_cast<qint64>(value) * TURNOVER_KILOS;

    if (mFilter.turnoverFrom > mFilter.turnoverTo)
    {
        mFilter.turnoverTo = mFilter.turnoverFrom;
        ui->turnoverToSpinBox->setValue(mFilter.turnoverTo / TURNOVER_KILOS);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_turnoverToSpinBox_valueChanged(int value)
{
    mFilter.turnoverTo = static_cast<qint64>(value) * TURNOVER_KILOS;

    if (mFilter.turnoverTo < mFilter.turnoverFrom)
    {
        mFilter.turnoverFrom = mFilter.turnoverTo;
        ui->turnoverFromSpinBox->setValue(mFilter.turnoverFrom / TURNOVER_KILOS);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_paybackCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mFilter.usePayback = checked;
    ui->paybackFromDoubleSpinBox->setEnabled(checked);
    ui->paybackToDoubleSpinBox->setEnabled(checked);

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_paybackFromDoubleSpinBox_valueChanged(double value)
{
    mFilter.paybackFrom = value;

    if (mFilter.paybackFrom > mFilter.paybackTo)
    {
        mFilter.paybackTo = mFilter.paybackFrom;
        ui->paybackToDoubleSpinBox->setValue(mFilter.paybackTo);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_paybackToDoubleSpinBox_valueChanged(double value)
{
    mFilter.paybackTo = value;

    if (mFilter.paybackTo < mFilter.paybackFrom)
    {
        mFilter.paybackFrom = mFilter.paybackTo;
        ui->paybackFromDoubleSpinBox->setValue(mFilter.paybackFrom);
    }

    filterChangeDelayTimer.start();
}

void StocksControlsWidget::on_hideButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == ui->controlsVisiblePage)
    {
        setMaximumSize(HIDDEN_WIDTH, MAXIMUM_HEIGHT);
        ui->stackedWidget->setCurrentWidget(ui->controlsHiddenPage);
        ui->hideButton->setIcon(QIcon(":/assets/images/right_arrows.png"));

        if (mFilter.isActive())
        {
            ui->filterActiveLabel->setColor(GREEN_COLOR);
            ui->filterActiveLabel->setText(tr("Filter active"));
        }
        else
        {
            ui->filterActiveLabel->setColor(GREY_COLOR);
            ui->filterActiveLabel->setText(tr("Filter inactive"));
        }
    }
    else
    {
        setMaximumSize(VISIBLE_WIDTH, MAXIMUM_HEIGHT);
        ui->stackedWidget->setCurrentWidget(ui->controlsVisiblePage);
        ui->hideButton->setIcon(QIcon(":/assets/images/left_arrows.png"));
    }
}

void StocksControlsWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/dateChangeTime",     ui->dateChangeTimeEdit->dateTime().toString(DATETIME_FORMAT));
    mSettingsEditor->setValue(type + "/useTicker",          ui->tickerCheckBox->isChecked());
    mSettingsEditor->setValue(type + "/ticker",             ui->tickerLineEdit->text());
    mSettingsEditor->setValue(type + "/useQualInvestor",    ui->qualInvestorCheckBox->isChecked());
    mSettingsEditor->setValue(type + "/qualInvestor",       ui->qualInvestorComboBox->currentIndex());
    mSettingsEditor->setValue(type + "/usePrice",           ui->priceCheckBox->isChecked());
    mSettingsEditor->setValue(type + "/priceFrom",          ui->priceFromDoubleSpinBox->value());
    mSettingsEditor->setValue(type + "/priceTo",            ui->priceToDoubleSpinBox->value());
    mSettingsEditor->setValue(type + "/useDayStartChange",  ui->dayStartChangeCheckBox->isChecked());
    mSettingsEditor->setValue(type + "/dayStartChangeFrom", ui->dayStartChangeFromDoubleSpinBox->value());
    mSettingsEditor->setValue(type + "/dayStartChangeTo",   ui->dayStartChangeToDoubleSpinBox->value());
    mSettingsEditor->setValue(type + "/useDateChange",      ui->dateChangeCheckBox->isChecked());
    mSettingsEditor->setValue(type + "/dateChangeFrom",     ui->dateChangeFromDoubleSpinBox->value());
    mSettingsEditor->setValue(type + "/dateChangeTo",       ui->dateChangeToDoubleSpinBox->value());
    mSettingsEditor->setValue(type + "/useTurnover",        ui->turnoverCheckBox->isChecked());
    mSettingsEditor->setValue(type + "/turnoverFrom",       ui->turnoverFromSpinBox->value());
    mSettingsEditor->setValue(type + "/turnoverTo",         ui->turnoverToSpinBox->value());
    mSettingsEditor->setValue(type + "/usePayback",         ui->paybackCheckBox->isChecked());
    mSettingsEditor->setValue(type + "/paybackFrom",        ui->paybackFromDoubleSpinBox->value());
    mSettingsEditor->setValue(type + "/paybackTo",          ui->paybackToDoubleSpinBox->value());
    // clang-format on

    mSettingsEditor->setValue(type + "/visible", ui->stackedWidget->currentWidget() == ui->controlsVisiblePage);
}

// NOLINTBEGIN(readability-magic-numbers)
void StocksControlsWidget::loadWindowState(const QString& type)
{
    const int currentYear = QDateTime::currentDateTime().date().year();

    const QString defaultDateChangeTime = QString("%1-01-01 00:00:00").arg(currentYear - 1);

    // clang-format off
    ui->dateChangeTimeEdit->setDateTime(QDateTime::fromString(mSettingsEditor->value(type + "/dateChangeTime", defaultDateChangeTime).toString(), DATETIME_FORMAT));
    ui->tickerCheckBox->setChecked(mSettingsEditor->value(type + "/useTicker",                                 false).toBool());
    ui->tickerLineEdit->setText(mSettingsEditor->value(type + "/ticker",                                       "").toString());
    ui->qualInvestorCheckBox->setChecked(mSettingsEditor->value(type + "/useQualInvestor",                     false).toBool());
    ui->qualInvestorComboBox->setCurrentIndex(mSettingsEditor->value(type + "/qualInvestor",                   QUAL_INVESTOR_SHOW_ALL).toInt());
    ui->priceCheckBox->setChecked(mSettingsEditor->value(type + "/usePrice",                                   false).toBool());
    ui->priceFromDoubleSpinBox->setValue(mSettingsEditor->value(type + "/priceFrom",                           0.0).toDouble());
    ui->priceToDoubleSpinBox->setValue(mSettingsEditor->value(type + "/priceTo",                               0.0).toDouble());
    ui->dayStartChangeCheckBox->setChecked(mSettingsEditor->value(type + "/useDayStartChange",                 false).toBool());
    ui->dayStartChangeFromDoubleSpinBox->setValue(mSettingsEditor->value(type + "/dayStartChangeFrom",         0.0).toDouble());
    ui->dayStartChangeToDoubleSpinBox->setValue(mSettingsEditor->value(type + "/dayStartChangeTo",             0.0).toDouble());
    ui->dateChangeCheckBox->setChecked(mSettingsEditor->value(type + "/useDateChange",                         false).toBool());
    ui->dateChangeFromDoubleSpinBox->setValue(mSettingsEditor->value(type + "/dateChangeFrom",                 0.0).toDouble());
    ui->dateChangeToDoubleSpinBox->setValue(mSettingsEditor->value(type + "/dateChangeTo",                     0.0).toDouble());
    ui->turnoverCheckBox->setChecked(mSettingsEditor->value(type + "/useTurnover",                             false).toBool());
    ui->turnoverFromSpinBox->setValue(mSettingsEditor->value(type + "/turnoverFrom",                           0).toLongLong());
    ui->turnoverToSpinBox->setValue(mSettingsEditor->value(type + "/turnoverTo",                               1000000000).toLongLong());
    ui->paybackCheckBox->setChecked(mSettingsEditor->value(type + "/usePayback",                               false).toBool());
    ui->paybackFromDoubleSpinBox->setValue(mSettingsEditor->value(type + "/paybackFrom",                       0.0).toDouble());
    ui->paybackToDoubleSpinBox->setValue(mSettingsEditor->value(type + "/paybackTo",                           100.0).toDouble());
    // clang-format on

    if (!mSettingsEditor->value(type + "/visible", true).toBool())
    {
        ui->hideButton->click();
    }
}
// NOLINTEND(readability-magic-numbers)
