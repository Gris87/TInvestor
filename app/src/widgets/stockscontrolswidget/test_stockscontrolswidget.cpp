#include "src/widgets/stockscontrolswidget/stockscontrolswidget.h"
#include "ui_stockscontrolswidget.h"

#include <gtest/gtest.h>

#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_StocksControlsWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        stocksStorageMock = new StrictMock<StocksStorageMock>();

        stocksControlsWidget = new StocksControlsWidget(stocksStorageMock);
    }

    void TearDown()
    {
        delete stocksControlsWidget;
        delete stocksStorageMock;
    }

    StocksControlsWidget* stocksControlsWidget;
    StrictMock<StocksStorageMock>* stocksStorageMock;
};



TEST_F(Test_StocksControlsWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksControlsWidget, Test_getFilter)
{
    Filter filter = stocksControlsWidget->getFilter();

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

TEST_F(Test_StocksControlsWidget, Test_on_tickerCheckBox_checkStateChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_tickerLineEdit_textChanged)
{
    stocksControlsWidget->ui->tickerLineEdit->blockSignals(true);
    stocksControlsWidget->ui->tickerLineEdit->setText("");
    stocksControlsWidget->ui->tickerLineEdit->blockSignals(false);

    stocksControlsWidget->ui->tickerLineEdit->setText("SPBE");
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.ticker, "SPBE");

    stocksControlsWidget->ui->tickerLineEdit->setText("RBCM");
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.ticker, "RBCM");
}

TEST_F(Test_StocksControlsWidget, Test_on_qualInvestorCheckBox_checkStateChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_qualInvestorComboBox_currentIndexChanged)
{
    stocksControlsWidget->ui->qualInvestorComboBox->blockSignals(true);
    stocksControlsWidget->ui->qualInvestorComboBox->setCurrentIndex(0);
    stocksControlsWidget->ui->qualInvestorComboBox->blockSignals(false);

    stocksControlsWidget->ui->qualInvestorComboBox->setCurrentIndex(1);
    Filter filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.qualInvestor, QUAL_INVESTOR_ONLY_WITH_STATUS);

    stocksControlsWidget->ui->qualInvestorComboBox->setCurrentIndex(2);
    filter = stocksControlsWidget->getFilter();

    ASSERT_EQ(filter.qualInvestor, QUAL_INVESTOR_ONLY_WITHOUT_STATUS);
}

TEST_F(Test_StocksControlsWidget, Test_on_priceCheckBox_checkStateChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_priceFromDoubleSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_priceToDoubleSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_dayStartChangeCheckBox_checkStateChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_dayStartChangeFromDoubleSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_dayStartChangeToDoubleSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_dateChangeCheckBox_checkStateChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_dateChangeFromDoubleSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_dateChangeToDoubleSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_turnoverCheckBox_checkStateChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_turnoverFromSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_turnoverToSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_paybackCheckBox_checkStateChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_paybackFromDoubleSpinBox_valueChanged)
{
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
}

TEST_F(Test_StocksControlsWidget, Test_on_paybackToDoubleSpinBox_valueChanged)
{
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
}
