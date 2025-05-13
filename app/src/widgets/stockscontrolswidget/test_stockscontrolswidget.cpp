#include "src/widgets/stockscontrolswidget/stockscontrolswidget.h"
#include "ui_stockscontrolswidget.h"

#include <gtest/gtest.h>

#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



#ifdef Q_OS_WINDOWS
const QSize DEFAULT_MAXIMUM_SIZE = QSize(250, 16777215);
#else
const QSize DEFAULT_MAXIMUM_SIZE = QSize(270, 16777215);
#endif



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_StocksControlsWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stocksStorageMock  = new StrictMock<StocksStorageMock>();
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        stocksControlsWidget = new StocksControlsWidget(stocksStorageMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete stocksControlsWidget;
        delete stocksStorageMock;
        delete settingsEditorMock;
    }

    StocksControlsWidget*           stocksControlsWidget;
    StrictMock<StocksStorageMock>*  stocksStorageMock;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_StocksControlsWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksControlsWidget, Test_getDateChangeTime)
{
    const QDateTime dateTime1(QDate(2024, 1, 1), QTime(0, 0, 0));
    const QDateTime dateTime2(QDate(2025, 1, 1), QTime(0, 0, 0));

    stocksControlsWidget->ui->dateChangeTimeEdit->setDateTime(dateTime1);
    ASSERT_EQ(stocksControlsWidget->getDateChangeTime(), dateTime1);

    stocksControlsWidget->ui->dateChangeTimeEdit->setDateTime(dateTime2);
    ASSERT_EQ(stocksControlsWidget->getDateChangeTime(), dateTime2);
}

TEST_F(Test_StocksControlsWidget, Test_getFilter)
{
    const Filter filter = stocksControlsWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.useTicker,            false);
    ASSERT_EQ(filter.ticker,               "");
    ASSERT_EQ(filter.useQualInvestor,      false);
    ASSERT_EQ(filter.qualInvestor,         QUAL_INVESTOR_SHOW_ALL);
    ASSERT_EQ(filter.usePrice,             false);
    ASSERT_NEAR(filter.priceFrom,          0.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo,            0.0f, 0.0001f);
    ASSERT_EQ(filter.useDayStartChange,    false);
    ASSERT_NEAR(filter.dayStartChangeFrom, 0.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo,   0.0f, 0.0001f);
    ASSERT_EQ(filter.useDateChange,        false);
    ASSERT_NEAR(filter.dateChangeFrom,     0.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo,       0.0f, 0.0001f);
    ASSERT_EQ(filter.useTurnover,          false);
    ASSERT_EQ(filter.turnoverFrom,         0);
    ASSERT_EQ(filter.turnoverTo,           1000000000000);
    ASSERT_EQ(filter.usePayback,           false);
    ASSERT_NEAR(filter.paybackFrom,        0.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo,          100.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksControlsWidget, Test_dateChangeDelayTimerTicked)
{
    const InSequence seq;

    const QDateTime dateTime(QDate(2024, 1, 1), QTime(0, 0, 0));

    ASSERT_EQ(stocksControlsWidget->dateChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dateChangeTimeEdit->setDateTime(dateTime);
    ASSERT_EQ(stocksControlsWidget->dateChangeDelayTimer.isActive(), true);

    stocksControlsWidget->dateChangeDelayTimerTicked();

    ASSERT_EQ(stocksControlsWidget->dateChangeDelayTimer.isActive(), false);
}

TEST_F(Test_StocksControlsWidget, Test_filterChangeDelayTimerTicked)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->filterChangeDelayTimer.start(100000);
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);

    stocksControlsWidget->filterChangeDelayTimerTicked();

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);
}

TEST_F(Test_StocksControlsWidget, Test_on_dateChangeTimeEdit_dateTimeChanged)
{
    const QDateTime dateTime1(QDate(2024, 1, 1), QTime(0, 0, 0));
    const QDateTime dateTime2(QDate(2025, 1, 1), QTime(0, 0, 0));

    ASSERT_EQ(stocksControlsWidget->dateChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dateChangeTimeEdit->setDateTime(dateTime1);
    ASSERT_EQ(stocksControlsWidget->dateChangeDelayTimer.isActive(), true);

    stocksControlsWidget->ui->dateChangeTimeEdit->setDateTime(dateTime2);
    ASSERT_EQ(stocksControlsWidget->dateChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_tickerCheckBox_checkStateChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->tickerCheckBox->blockSignals(true);
    stocksControlsWidget->ui->tickerCheckBox->setChecked(false);
    stocksControlsWidget->ui->tickerCheckBox->blockSignals(false);

    stocksControlsWidget->ui->tickerCheckBox->setChecked(true);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->tickerLineEdit->isEnabled(), true);
    ASSERT_EQ(filter.useTicker, true);

    stocksControlsWidget->ui->tickerCheckBox->setChecked(false);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->tickerLineEdit->isEnabled(), false);
    ASSERT_EQ(filter.useTicker, false);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_tickerLineEdit_textChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->tickerLineEdit->blockSignals(true);
    stocksControlsWidget->ui->tickerLineEdit->setText("");
    stocksControlsWidget->ui->tickerLineEdit->blockSignals(false);

    stocksControlsWidget->ui->tickerLineEdit->setText("SPBE");
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.ticker, "SPBE");

    stocksControlsWidget->ui->tickerLineEdit->setText("RBCM");
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.ticker, "RBCM");

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_qualInvestorCheckBox_checkStateChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->qualInvestorCheckBox->blockSignals(true);
    stocksControlsWidget->ui->qualInvestorCheckBox->setChecked(false);
    stocksControlsWidget->ui->qualInvestorCheckBox->blockSignals(false);

    stocksControlsWidget->ui->qualInvestorCheckBox->setChecked(true);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->qualInvestorComboBox->isEnabled(), true);
    ASSERT_EQ(filter.useQualInvestor, true);

    stocksControlsWidget->ui->qualInvestorCheckBox->setChecked(false);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->qualInvestorComboBox->isEnabled(), false);
    ASSERT_EQ(filter.useQualInvestor, false);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_qualInvestorComboBox_currentIndexChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->qualInvestorComboBox->blockSignals(true);
    stocksControlsWidget->ui->qualInvestorComboBox->setCurrentIndex(0);
    stocksControlsWidget->ui->qualInvestorComboBox->blockSignals(false);

    stocksControlsWidget->ui->qualInvestorComboBox->setCurrentIndex(1);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.qualInvestor, QUAL_INVESTOR_ONLY_WITH_STATUS);

    stocksControlsWidget->ui->qualInvestorComboBox->setCurrentIndex(2);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.qualInvestor, QUAL_INVESTOR_ONLY_WITHOUT_STATUS);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_priceCheckBox_checkStateChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->priceCheckBox->blockSignals(true);
    stocksControlsWidget->ui->priceCheckBox->setChecked(false);
    stocksControlsWidget->ui->priceCheckBox->blockSignals(false);

    stocksControlsWidget->ui->priceCheckBox->setChecked(true);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->priceFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(stocksControlsWidget->ui->priceToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.usePrice, true);

    stocksControlsWidget->ui->priceCheckBox->setChecked(false);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->priceFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(stocksControlsWidget->ui->priceToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.usePrice, false);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_priceFromDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->priceFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->priceToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->priceFromDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->priceToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 4.0f, 0.0001f);

    stocksControlsWidget->ui->priceFromDoubleSpinBox->setValue(5.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->priceToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 5.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_priceToDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->priceFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->priceToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->priceToDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->priceFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 3.0f, 0.0001f);

    stocksControlsWidget->ui->priceToDoubleSpinBox->setValue(1.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->priceFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 1.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_dayStartChangeCheckBox_checkStateChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dayStartChangeCheckBox->blockSignals(true);
    stocksControlsWidget->ui->dayStartChangeCheckBox->setChecked(false);
    stocksControlsWidget->ui->dayStartChangeCheckBox->blockSignals(false);

    stocksControlsWidget->ui->dayStartChangeCheckBox->setChecked(true);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.useDayStartChange, true);

    stocksControlsWidget->ui->dayStartChangeCheckBox->setChecked(false);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.useDayStartChange, false);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_dayStartChangeFromDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 4.0f, 0.0001f);

    stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(5.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 5.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_dayStartChangeToDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 3.0f, 0.0001f);

    stocksControlsWidget->ui->dayStartChangeToDoubleSpinBox->setValue(1.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dayStartChangeFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 1.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_dateChangeCheckBox_checkStateChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dateChangeCheckBox->blockSignals(true);
    stocksControlsWidget->ui->dateChangeCheckBox->setChecked(false);
    stocksControlsWidget->ui->dateChangeCheckBox->blockSignals(false);

    stocksControlsWidget->ui->dateChangeCheckBox->setChecked(true);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(stocksControlsWidget->ui->dateChangeToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.useDateChange, true);

    stocksControlsWidget->ui->dateChangeCheckBox->setChecked(false);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(stocksControlsWidget->ui->dateChangeToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.useDateChange, false);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_dateChangeFromDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->dateChangeToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dateChangeToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 4.0f, 0.0001f);

    stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->setValue(5.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dateChangeToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 5.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_dateChangeToDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->dateChangeToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->dateChangeToDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 3.0f, 0.0001f);

    stocksControlsWidget->ui->dateChangeToDoubleSpinBox->setValue(1.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->dateChangeFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 1.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_turnoverCheckBox_checkStateChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->turnoverCheckBox->blockSignals(true);
    stocksControlsWidget->ui->turnoverCheckBox->setChecked(false);
    stocksControlsWidget->ui->turnoverCheckBox->blockSignals(false);

    stocksControlsWidget->ui->turnoverCheckBox->setChecked(true);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->turnoverFromSpinBox->isEnabled(), true);
    ASSERT_EQ(stocksControlsWidget->ui->turnoverToSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.useTurnover, true);

    stocksControlsWidget->ui->turnoverCheckBox->setChecked(false);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->turnoverFromSpinBox->isEnabled(), false);
    ASSERT_EQ(stocksControlsWidget->ui->turnoverToSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.useTurnover, false);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_turnoverFromSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->turnoverFromSpinBox->setValue(2);
    stocksControlsWidget->ui->turnoverToSpinBox->setValue(4);

    stocksControlsWidget->ui->turnoverFromSpinBox->setValue(3);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->turnoverToSpinBox->value(), 4);
    ASSERT_EQ(filter.turnoverFrom, 3000);
    ASSERT_EQ(filter.turnoverTo, 4000);

    stocksControlsWidget->ui->turnoverFromSpinBox->setValue(5);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->turnoverToSpinBox->value(), 5);
    ASSERT_EQ(filter.turnoverFrom, 5000);
    ASSERT_EQ(filter.turnoverTo, 5000);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_turnoverToSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->turnoverFromSpinBox->setValue(2);
    stocksControlsWidget->ui->turnoverToSpinBox->setValue(4);

    stocksControlsWidget->ui->turnoverToSpinBox->setValue(3);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->turnoverFromSpinBox->value(), 2);
    ASSERT_EQ(filter.turnoverFrom, 2000);
    ASSERT_EQ(filter.turnoverTo, 3000);

    stocksControlsWidget->ui->turnoverToSpinBox->setValue(1);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->turnoverFromSpinBox->value(), 1);
    ASSERT_EQ(filter.turnoverFrom, 1000);
    ASSERT_EQ(filter.turnoverTo, 1000);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_paybackCheckBox_checkStateChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->paybackCheckBox->blockSignals(true);
    stocksControlsWidget->ui->paybackCheckBox->setChecked(false);
    stocksControlsWidget->ui->paybackCheckBox->blockSignals(false);

    stocksControlsWidget->ui->paybackCheckBox->setChecked(true);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->paybackFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(stocksControlsWidget->ui->paybackToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.usePayback, true);

    stocksControlsWidget->ui->paybackCheckBox->setChecked(false);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(stocksControlsWidget->ui->paybackFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(stocksControlsWidget->ui->paybackToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.usePayback, false);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_paybackFromDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->paybackFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->paybackToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->paybackFromDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->paybackToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 4.0f, 0.0001f);

    stocksControlsWidget->ui->paybackFromDoubleSpinBox->setValue(5.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->paybackToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 5.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_paybackToDoubleSpinBox_valueChanged)
{
    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), false);

    stocksControlsWidget->ui->paybackFromDoubleSpinBox->setValue(2.0);
    stocksControlsWidget->ui->paybackToDoubleSpinBox->setValue(4.0);

    stocksControlsWidget->ui->paybackToDoubleSpinBox->setValue(3.0);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->paybackFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 3.0f, 0.0001f);

    stocksControlsWidget->ui->paybackToDoubleSpinBox->setValue(1.0);
    filter = stocksControlsWidget->getFilter();

    ASSERT_NEAR(stocksControlsWidget->ui->paybackFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 1.0f, 0.0001f);

    ASSERT_EQ(stocksControlsWidget->filterChangeDelayTimer.isActive(), true);
}

TEST_F(Test_StocksControlsWidget, Test_on_hideButton_clicked)
{
    ASSERT_EQ(stocksControlsWidget->maximumSize(), DEFAULT_MAXIMUM_SIZE);
    ASSERT_EQ(stocksControlsWidget->ui->stackedWidget->currentWidget(), stocksControlsWidget->ui->controlsVisiblePage);

    stocksControlsWidget->ui->hideButton->click();

    // clang-format off
    ASSERT_EQ(stocksControlsWidget->maximumSize(),                      QSize(24, 16777215));
    ASSERT_EQ(stocksControlsWidget->ui->stackedWidget->currentWidget(), stocksControlsWidget->ui->controlsHiddenPage);
    ASSERT_EQ(stocksControlsWidget->ui->filterActiveLabel->color(),     QColor("#AFC2D7"));
    ASSERT_EQ(stocksControlsWidget->ui->filterActiveLabel->text(),      "Filter inactive");
    // clang-format on

    stocksControlsWidget->ui->hideButton->click();

    ASSERT_EQ(stocksControlsWidget->maximumSize(), DEFAULT_MAXIMUM_SIZE);
    ASSERT_EQ(stocksControlsWidget->ui->stackedWidget->currentWidget(), stocksControlsWidget->ui->controlsVisiblePage);

    stocksControlsWidget->ui->priceCheckBox->setChecked(true);
    stocksControlsWidget->ui->hideButton->click();

    // clang-format off
    ASSERT_EQ(stocksControlsWidget->maximumSize(),                      QSize(24, 16777215));
    ASSERT_EQ(stocksControlsWidget->ui->stackedWidget->currentWidget(), stocksControlsWidget->ui->controlsHiddenPage);
    ASSERT_EQ(stocksControlsWidget->ui->filterActiveLabel->color(),     QColor("#2BD793"));
    ASSERT_EQ(stocksControlsWidget->ui->filterActiveLabel->text(),      "Filter active");
    // clang-format on
}

TEST_F(Test_StocksControlsWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/dateChangeTime"),     QVariant("2025-01-01 00:00:00")));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/useTicker"),          QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/ticker"),             QVariant("")));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/useQualInvestor"),    QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/qualInvestor"),       QVariant(QUAL_INVESTOR_SHOW_ALL)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/usePrice"),           QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/priceFrom"),          QVariant(0.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/priceTo"),            QVariant(0.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/useDayStartChange"),  QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/dayStartChangeFrom"), QVariant(0.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/dayStartChangeTo"),   QVariant(0.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/useDateChange"),      QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/dateChangeFrom"),     QVariant(0.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/dateChangeTo"),       QVariant(0.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/useTurnover"),        QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/turnoverFrom"),       QVariant(0)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/turnoverTo"),         QVariant(1000000000)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/usePayback"),         QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/paybackFrom"),        QVariant(0.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/paybackTo"),          QVariant(100.0f)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/visible"),            QVariant(true)));
    // clang-format on

    stocksControlsWidget->saveWindowState("AAAAA");
}

TEST_F(Test_StocksControlsWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/dateChangeTime"),     QVariant("2024-01-01 00:00:00"))).WillOnce(Return(QVariant("2024-01-01 00:00:00")));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/useTicker"),          QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/ticker"),             QVariant(""))).WillOnce(Return(QVariant("")));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/useQualInvestor"),    QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/qualInvestor"),       QVariant(QUAL_INVESTOR_SHOW_ALL))).WillOnce(Return(QVariant(QUAL_INVESTOR_SHOW_ALL)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/usePrice"),           QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/priceFrom"),          QVariant(0.0))).WillOnce(Return(QVariant(0.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/priceTo"),            QVariant(0.0))).WillOnce(Return(QVariant(0.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/useDayStartChange"),  QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/dayStartChangeFrom"), QVariant(0.0))).WillOnce(Return(QVariant(0.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/dayStartChangeTo"),   QVariant(0.0))).WillOnce(Return(QVariant(0.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/useDateChange"),      QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/dateChangeFrom"),     QVariant(0.0))).WillOnce(Return(QVariant(0.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/dateChangeTo"),       QVariant(0.0))).WillOnce(Return(QVariant(0.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/useTurnover"),        QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/turnoverFrom"),       QVariant(0))).WillOnce(Return(QVariant(0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/turnoverTo"),         QVariant(1000000000))).WillOnce(Return(QVariant(1000000000)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/usePayback"),         QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/paybackFrom"),        QVariant(0.0))).WillOnce(Return(QVariant(0.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/paybackTo"),          QVariant(100.0))).WillOnce(Return(QVariant(100.0)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/visible"),            QVariant(true))).WillOnce(Return(QVariant(false)));
    // clang-format on

    stocksControlsWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
