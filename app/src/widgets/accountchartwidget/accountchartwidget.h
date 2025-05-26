#pragma once



#include "src/widgets/accountchartwidget/iaccountchartwidget.h"

#include <QChart>
#include <QDateTimeAxis>
#include <QLineSeries>
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

    QTimer tooltipHideTimer;

    void zoom(double factor);

    bool eventFilter(QObject* object, QEvent* event) override;

    void switchChart(ChartType chartType) override;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;

public slots:
    void contextMenuRequested(const QPoint& pos);
    void actionExportToExcelTriggered();
    void lineSeriesHovered(QPointF point, bool state);
    void tooltipHideTimerTicked();

private:
    void    initYieldChart();
    void    initMonthlyYieldChart();
    void    initRemainedMoneyChart();
    void    initTotalMoneyChart();
    void    initChartStyle(QChart* chart, QAbstractAxis* axisX, QAbstractAxis* axisY);
    void    handleOperation(const Operation& operation);
    void    exportToExcel(const QString& path) const;
    QPointF findNearestPoint(const QPointF& point, const QList<QPointF>& seriesPoints);

    IFileDialogFactory* mFileDialogFactory;
    ISettingsEditor*    mSettingsEditor;
    ChartType           mChartType;
    QChart              mYieldChart;
    QLineSeries         mYieldSeries;
    QDateTimeAxis       mYieldAxisX;
    QValueAxis          mYieldAxisY;
    QChart              mMonthlyYieldChart;
    QLineSeries         mMonthlyYieldSeries;
    QDateTimeAxis       mMonthlyYieldAxisX;
    QValueAxis          mMonthlyYieldAxisY;
    QChart              mRemainedMoneyChart;
    QLineSeries         mRemainedMoneySeries;
    QDateTimeAxis       mRemainedMoneyAxisX;
    QValueAxis          mRemainedMoneyAxisY;
    QChart              mTotalMoneyChart;
    QLineSeries         mTotalMoneySeries;
    QDateTimeAxis       mTotalMoneyAxisX;
    QValueAxis          mTotalMoneyAxisY;
    int                 mLastMonthlyCategory;
    float               mLastMonthlyYield;
    qint64              mAxisXMin;
    qint64              mAxisXMax;
    float               mYieldAxisYMin;
    float               mYieldAxisYMax;
    float               mMonthlyYieldAxisYMin;
    float               mMonthlyYieldAxisYMax;
    float               mRemainedMoneyAxisYMin;
    float               mRemainedMoneyAxisYMax;
    float               mTotalMoneyAxisYMin;
    float               mTotalMoneyAxisYMax;
    QPointF             mTargetScenePos;
    QPointF             mTargetViewportPos;
    ChartTooltip*       mTooltip;
};
