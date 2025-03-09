#pragma once



#include "src/widgets/filterwidget/ifilterwidget.h"



namespace Ui
{
class FilterWidget;
}



class FilterWidget : public IFilterWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget* parent = nullptr);
    ~FilterWidget();

    FilterWidget(const FilterWidget& another)            = delete;
    FilterWidget& operator=(const FilterWidget& another) = delete;

    Ui::FilterWidget* ui;

private:
    Filter mFilter;

private slots:
    void on_tickerCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_tickerLineEdit_editingFinished();
    void on_priceCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_priceFromDoubleSpinBox_editingFinished();
    void on_priceToDoubleSpinBox_editingFinished();
    void on_dayStartChangeCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_dayStartChangeFromDoubleSpinBox_editingFinished();
    void on_dayStartChangeToDoubleSpinBox_editingFinished();
    void on_dateChangeCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_dateChangeFromDoubleSpinBox_editingFinished();
    void on_dateChangeToDoubleSpinBox_editingFinished();
    void on_paybackCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_paybackFromDoubleSpinBox_editingFinished();
    void on_paybackToDoubleSpinBox_editingFinished();
};
