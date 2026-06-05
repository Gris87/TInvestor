#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChart>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QStackedBarSeries>
#include <QTimer>
#include <QValueAxis>

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/charttooltip.h"



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

    QTimer        tooltipHideTimer;
    ChartTooltip* tooltip;

    void zoom(double factor);

    bool eventFilter(QObject* object, QEvent* event) override;

    void setShowMoney(bool value) override;

    void switchChart(ChartType chartType) override;
    void setTimeRange(TimeRange range) override;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;

    void syncTimeRangeSeries();
    void syncYieldTimeRangeSeries(TimeRange timeRange);
    void syncTotalMoneyTimeRangeSeries(TimeRange timeRange);
    void notifyAvgYield();

public slots:
    void contextMenuRequested(const QPoint& pos);
    void actionExportToExcelTriggered();
    void lineSeriesHovered(QPointF point, bool state);
    void barSeriesHovered(bool status, int index, QBarSet* barSet);
    void tooltipHideTimerTicked();

private:
    void    initYieldChart();
    void    initMonthlyYieldChart();
    void    initDailyYieldChart();
    void    initTotalMoneyChart();
    void    initChartStyle(QChart* chart, QAbstractAxis* axisX, QAbstractAxis* axisY);
    void    handleOperation(const Operation& operation, QList<QPointF>& yieldPoints, QList<QPointF>& totalMoneyPoints);
    void    syncBarSetFromPoints(QBarSet* barSet, const QList<qreal>& points);
    void    exportToExcel(const QString& path) const;
    QPointF findNearestPoint(const QPointF& point, const QList<QPointF>& seriesPoints);

    IFileDialogFactory* mFileDialogFactory;
    ISettingsEditor*    mSettingsEditor;
    ChartType           mChartType;
    TimeRange           mTimeRange;
    QChart              mYieldChart;
    QLineSeries         mYieldSeries;
    QDateTimeAxis       mYieldAxisX;
    QValueAxis          mYieldAxisY;
    QChart              mMonthlyYieldChart;
    QStackedBarSeries   mMonthlyYieldSeries;
    QBarSet             mMonthlyYieldPositiveBarSet;
    QBarSet             mMonthlyYieldNegativeBarSet;
    QList<qreal>        mMonthlyYieldPositivePoints;
    QList<qreal>        mMonthlyYieldNegativePoints;
    QBarCategoryAxis    mMonthlyYieldAxisX;
    QValueAxis          mMonthlyYieldAxisY;
    QChart              mDailyYieldChart;
    QStackedBarSeries   mDailyYieldSeries;
    QBarSet             mDailyYieldPositiveBarSet;
    QBarSet             mDailyYieldNegativeBarSet;
    QList<qreal>        mDailyYieldPositivePoints;
    QList<qreal>        mDailyYieldNegativePoints;
    QBarCategoryAxis    mDailyYieldAxisX;
    QValueAxis          mDailyYieldAxisY;
    QChart              mTotalMoneyChart;
    QLineSeries         mTotalMoneySeries;
    QDateTimeAxis       mTotalMoneyAxisX;
    QValueAxis          mTotalMoneyAxisY;
    QStringList         mMonthNames;
    bool                mShowMoney;
    qint64              mLastMonthLimitsStart;
    qint64              mLastMonthLimitsEnd;
    float               mLastMonthlyYield;
    qint64              mLastDayLimitsStart;
    qint64              mLastDayLimitsEnd;
    float               mLastDailyYield;
    qint64              mAxisXMin[TIME_RANGE_COUNT];
    qint64              mAxisXMax;
    float               mYieldAxisYMin[TIME_RANGE_COUNT];
    float               mYieldAxisYMax[TIME_RANGE_COUNT];
    float               mMonthlyYieldAxisYMin;
    float               mMonthlyYieldAxisYMax;
    float               mDailyYieldAxisYMin;
    float               mDailyYieldAxisYMax;
    float               mTotalMoneyAxisYMin[TIME_RANGE_COUNT];
    float               mTotalMoneyAxisYMax[TIME_RANGE_COUNT];
    QPointF             mTargetScenePos;
    QPointF             mTargetViewportPos;
};
