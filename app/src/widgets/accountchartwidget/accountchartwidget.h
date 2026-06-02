#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"

#include <QtGraphs>

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui { class AccountChartWidget; }



class AccountChartWidget : public IAccountChartWidget
{
    Q_OBJECT

public:
    explicit AccountChartWidget(
        IFileDialogFactory* fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent = nullptr
    );
    ~AccountChartWidget() override;

    AccountChartWidget(const AccountChartWidget& another)            = delete;
    AccountChartWidget& operator=(const AccountChartWidget& another) = delete;

    void setShowMoney(bool value) override;

    void switchChart(ChartType chartType) override;
    void setTimeRange(TimeRange range) override;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;

    Ui::AccountChartWidget* ui;

private:
    void initYieldChart();
    void initMonthlyYieldChart();
    void initDailyYieldChart();
    void initTotalMoneyChart();

    IFileDialogFactory* mFileDialogFactory;
    ISettingsEditor*    mSettingsEditor;
    QLineSeries         mYieldSeries;
    QDateTimeAxis       mYieldAxisX;
    QValueAxis          mYieldAxisY;
    QBarSeries          mMonthlyYieldSeries;
    QBarSet             mMonthlyYieldPositiveBarSet;
    QBarSet             mMonthlyYieldNegativeBarSet;
    QList<qreal>        mMonthlyYieldPositivePoints;
    QList<qreal>        mMonthlyYieldNegativePoints;
    QBarCategoryAxis    mMonthlyYieldAxisX;
    QValueAxis          mMonthlyYieldAxisY;
    QBarSeries          mDailyYieldSeries;
    QBarSet             mDailyYieldPositiveBarSet;
    QBarSet             mDailyYieldNegativeBarSet;
    QList<qreal>        mDailyYieldPositivePoints;
    QList<qreal>        mDailyYieldNegativePoints;
    QBarCategoryAxis    mDailyYieldAxisX;
    QValueAxis          mDailyYieldAxisY;
    QLineSeries         mTotalMoneySeries;
    QDateTimeAxis       mTotalMoneyAxisX;
    QValueAxis          mTotalMoneyAxisY;
    QStringList         mMonthNames;
};
