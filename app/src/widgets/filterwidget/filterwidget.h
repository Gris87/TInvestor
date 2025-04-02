#pragma once



#include "src/widgets/filterwidget/ifilterwidget.h"

#include "src/storage/stocks/istocksstorage.h"



namespace Ui
{
class FilterWidget;
}



class FilterWidget : public IFilterWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(IStocksStorage* stocksStorage, QWidget* parent = nullptr);
    ~FilterWidget();

    FilterWidget(const FilterWidget& another)            = delete;
    FilterWidget& operator=(const FilterWidget& another) = delete;

    Ui::FilterWidget* ui;

    const Filter& getFilter() override;

private:
    IStocksStorage* mStocksStorage;
    Filter mFilter;

private slots:
    void on_dateChangeTimeEdit_dateTimeChanged(const QDateTime& dateTime);
    void on_tickerCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_tickerLineEdit_textChanged(const QString& text);
    void on_qualInvestorCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_qualInvestorComboBox_currentIndexChanged(int index);
    void on_priceCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_priceFromDoubleSpinBox_valueChanged(double value);
    void on_priceToDoubleSpinBox_valueChanged(double value);
    void on_dayStartChangeCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_dayStartChangeFromDoubleSpinBox_valueChanged(double value);
    void on_dayStartChangeToDoubleSpinBox_valueChanged(double value);
    void on_dateChangeCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_dateChangeFromDoubleSpinBox_valueChanged(double value);
    void on_dateChangeToDoubleSpinBox_valueChanged(double value);
    void on_turnoverCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_turnoverFromSpinBox_valueChanged(int value);
    void on_turnoverToSpinBox_valueChanged(int value);
    void on_paybackCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_paybackFromDoubleSpinBox_valueChanged(double value);
    void on_paybackToDoubleSpinBox_valueChanged(double value);
    void on_hideStocksControlsButton_clicked();
};
