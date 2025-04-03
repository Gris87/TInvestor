#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidget.h"

#include <QTimer>

#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class StocksControlsWidget;
}



class StocksControlsWidget : public IStocksControlsWidget
{
    Q_OBJECT

public:
    explicit StocksControlsWidget(IStocksStorage* stocksStorage, ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~StocksControlsWidget();

    StocksControlsWidget(const StocksControlsWidget& another)            = delete;
    StocksControlsWidget& operator=(const StocksControlsWidget& another) = delete;

    Ui::StocksControlsWidget* ui;

    QDateTime     getDateChangeTime() override;
    const Filter& getFilter() override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    IStocksStorage*  mStocksStorage;
    ISettingsEditor* mSettingsEditor;
    QTimer           mDateChangeDelayTimer;
    QTimer           mFilterChangeDelayTimer;
    Filter           mFilter;

public slots:
    void dateChangeDelayTimerTicked();
    void filterChangeDelayTimerTicked();

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
    void on_hideButton_clicked();
};
