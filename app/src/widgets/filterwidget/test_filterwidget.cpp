#include "src/widgets/filterwidget/filterwidget.h"
#include "ui_filterwidget.h"

#include <gtest/gtest.h>



class Test_FilterWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        filterWidget = new FilterWidget();
    }

    void TearDown()
    {
        delete filterWidget;
    }

    FilterWidget* filterWidget;
};



TEST_F(Test_FilterWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_FilterWidget, Test_getFilter)
{
    Filter filter = filterWidget->getFilter();

    // clang-format off
    ASSERT_EQ(filter.useTicker,            false);
    ASSERT_EQ(filter.ticker,               "");
    ASSERT_EQ(filter.usePrice,             false);
    ASSERT_NEAR(filter.priceFrom,          0.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo,            0.0f, 0.0001f);
    ASSERT_EQ(filter.useDayStartChange,    false);
    ASSERT_NEAR(filter.dayStartChangeFrom, 0.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo,   0.0f, 0.0001f);
    ASSERT_EQ(filter.useDateChange,        false);
    ASSERT_NEAR(filter.dateChangeFrom,     0.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo,       0.0f, 0.0001f);
    ASSERT_EQ(filter.usePayback,           false);
    ASSERT_NEAR(filter.paybackFrom,        0.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo,          100.0f, 0.0001f);
    // clang-format on
}

TEST_F(Test_FilterWidget, Test_on_tickerCheckBox_checkStateChanged)
{
    filterWidget->ui->tickerCheckBox->blockSignals(true);
    filterWidget->ui->tickerCheckBox->setChecked(false);
    filterWidget->ui->tickerCheckBox->blockSignals(false);

    filterWidget->ui->tickerCheckBox->setChecked(true);
    Filter filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->tickerLineEdit->isEnabled(), true);
    ASSERT_EQ(filter.useTicker, true);

    filterWidget->ui->tickerCheckBox->setChecked(false);
    filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->tickerLineEdit->isEnabled(), false);
    ASSERT_EQ(filter.useTicker, false);
}

TEST_F(Test_FilterWidget, Test_on_tickerLineEdit_textChanged)
{
    filterWidget->ui->tickerLineEdit->blockSignals(true);
    filterWidget->ui->tickerLineEdit->setText("");
    filterWidget->ui->tickerLineEdit->blockSignals(false);

    filterWidget->ui->tickerLineEdit->setText("SPBE");
    Filter filter = filterWidget->getFilter();

    ASSERT_EQ(filter.ticker, "SPBE");

    filterWidget->ui->tickerLineEdit->setText("RBCM");
    filter = filterWidget->getFilter();

    ASSERT_EQ(filter.ticker, "RBCM");
}

TEST_F(Test_FilterWidget, Test_on_priceCheckBox_checkStateChanged)
{
    filterWidget->ui->priceCheckBox->blockSignals(true);
    filterWidget->ui->priceCheckBox->setChecked(false);
    filterWidget->ui->priceCheckBox->blockSignals(false);

    filterWidget->ui->priceCheckBox->setChecked(true);
    Filter filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->priceFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filterWidget->ui->priceToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.usePrice, true);

    filterWidget->ui->priceCheckBox->setChecked(false);
    filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->priceFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filterWidget->ui->priceToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.usePrice, false);
}

TEST_F(Test_FilterWidget, Test_on_priceFromDoubleSpinBox_valueChanged)
{
    filterWidget->ui->priceFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->priceToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->priceFromDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->priceToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 4.0f, 0.0001f);

    filterWidget->ui->priceFromDoubleSpinBox->setValue(5.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->priceToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 5.0f, 0.0001f);
}

TEST_F(Test_FilterWidget, Test_on_priceToDoubleSpinBox_valueChanged)
{
    filterWidget->ui->priceFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->priceToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->priceToDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->priceFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 3.0f, 0.0001f);

    filterWidget->ui->priceToDoubleSpinBox->setValue(1.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->priceFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.priceFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.priceTo, 1.0f, 0.0001f);
}

TEST_F(Test_FilterWidget, Test_on_dayStartChangeCheckBox_checkStateChanged)
{
    filterWidget->ui->dayStartChangeCheckBox->blockSignals(true);
    filterWidget->ui->dayStartChangeCheckBox->setChecked(false);
    filterWidget->ui->dayStartChangeCheckBox->blockSignals(false);

    filterWidget->ui->dayStartChangeCheckBox->setChecked(true);
    Filter filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->dayStartChangeFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filterWidget->ui->dayStartChangeToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.useDayStartChange, true);

    filterWidget->ui->dayStartChangeCheckBox->setChecked(false);
    filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->dayStartChangeFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filterWidget->ui->dayStartChangeToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.useDayStartChange, false);
}

TEST_F(Test_FilterWidget, Test_on_dayStartChangeFromDoubleSpinBox_valueChanged)
{
    filterWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->dayStartChangeToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dayStartChangeToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 4.0f, 0.0001f);

    filterWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(5.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dayStartChangeToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 5.0f, 0.0001f);
}

TEST_F(Test_FilterWidget, Test_on_dayStartChangeToDoubleSpinBox_valueChanged)
{
    filterWidget->ui->dayStartChangeFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->dayStartChangeToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->dayStartChangeToDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dayStartChangeFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 3.0f, 0.0001f);

    filterWidget->ui->dayStartChangeToDoubleSpinBox->setValue(1.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dayStartChangeFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dayStartChangeTo, 1.0f, 0.0001f);
}

TEST_F(Test_FilterWidget, Test_on_dateChangeCheckBox_checkStateChanged)
{
    filterWidget->ui->dateChangeCheckBox->blockSignals(true);
    filterWidget->ui->dateChangeCheckBox->setChecked(false);
    filterWidget->ui->dateChangeCheckBox->blockSignals(false);

    filterWidget->ui->dateChangeCheckBox->setChecked(true);
    Filter filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->dateChangeFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filterWidget->ui->dateChangeToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.useDateChange, true);

    filterWidget->ui->dateChangeCheckBox->setChecked(false);
    filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->dateChangeFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filterWidget->ui->dateChangeToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.useDateChange, false);
}

TEST_F(Test_FilterWidget, Test_on_dateChangeFromDoubleSpinBox_valueChanged)
{
    filterWidget->ui->dateChangeFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->dateChangeToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->dateChangeFromDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dateChangeToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 4.0f, 0.0001f);

    filterWidget->ui->dateChangeFromDoubleSpinBox->setValue(5.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dateChangeToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 5.0f, 0.0001f);
}

TEST_F(Test_FilterWidget, Test_on_dateChangeToDoubleSpinBox_valueChanged)
{
    filterWidget->ui->dateChangeFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->dateChangeToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->dateChangeToDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dateChangeFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 3.0f, 0.0001f);

    filterWidget->ui->dateChangeToDoubleSpinBox->setValue(1.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->dateChangeFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.dateChangeTo, 1.0f, 0.0001f);
}

TEST_F(Test_FilterWidget, Test_on_paybackCheckBox_checkStateChanged)
{
    filterWidget->ui->paybackCheckBox->blockSignals(true);
    filterWidget->ui->paybackCheckBox->setChecked(false);
    filterWidget->ui->paybackCheckBox->blockSignals(false);

    filterWidget->ui->paybackCheckBox->setChecked(true);
    Filter filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->paybackFromDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filterWidget->ui->paybackToDoubleSpinBox->isEnabled(), true);
    ASSERT_EQ(filter.usePayback, true);

    filterWidget->ui->paybackCheckBox->setChecked(false);
    filter = filterWidget->getFilter();

    ASSERT_EQ(filterWidget->ui->paybackFromDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filterWidget->ui->paybackToDoubleSpinBox->isEnabled(), false);
    ASSERT_EQ(filter.usePayback, false);
}

TEST_F(Test_FilterWidget, Test_on_paybackFromDoubleSpinBox_valueChanged)
{
    filterWidget->ui->paybackFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->paybackToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->paybackFromDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->paybackToDoubleSpinBox->value(), 4.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 3.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 4.0f, 0.0001f);

    filterWidget->ui->paybackFromDoubleSpinBox->setValue(5.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->paybackToDoubleSpinBox->value(), 5.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 5.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 5.0f, 0.0001f);
}

TEST_F(Test_FilterWidget, Test_on_paybackToDoubleSpinBox_valueChanged)
{
    filterWidget->ui->paybackFromDoubleSpinBox->setValue(2.0);
    filterWidget->ui->paybackToDoubleSpinBox->setValue(4.0);

    filterWidget->ui->paybackToDoubleSpinBox->setValue(3.0);
    Filter filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->paybackFromDoubleSpinBox->value(), 2.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 2.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 3.0f, 0.0001f);

    filterWidget->ui->paybackToDoubleSpinBox->setValue(1.0);
    filter = filterWidget->getFilter();

    ASSERT_NEAR(filterWidget->ui->paybackFromDoubleSpinBox->value(), 1.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackFrom, 1.0f, 0.0001f);
    ASSERT_NEAR(filter.paybackTo, 1.0f, 0.0001f);
}
