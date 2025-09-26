#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>
#include <QGraphicsLayout>
#include <QMenu>

#include "src/grpc/utils.h"
#include "src/qxlsx/xlsxchart.h"
#include "src/qxlsx/xlsxchartsheet.h"
#include "src/qxlsx/xlsxdocument.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const DATETIME_FORMAT         = "yyyy-MM-dd hh:mm:ss";
const char* const DATE_FORMAT             = "yyyy-MM-dd";
const QColor      BACKGROUND_COLOR        = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      PLOT_AREA_COLOR         = QColor("#344759"); // clazy:exclude=non-pod-global-static
const QColor      TITLE_COLOR             = QColor("#FFFFFF"); // clazy:exclude=non-pod-global-static
const QColor      LABEL_COLOR             = QColor("#AFC2D7"); // clazy:exclude=non-pod-global-static
const QColor      AXIS_COLOR              = QColor("#FFFFFF"); // clazy:exclude=non-pod-global-static
const QColor      GRID_COLOR              = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      SERIES_COLOR            = QColor("#6D85FF"); // clazy:exclude=non-pod-global-static
const QColor      GREEN_COLOR             = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor      RED_COLOR               = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor      HEADER_BACKGROUND_COLOR = QColor("#354450"); // clazy:exclude=non-pod-global-static
const QColor      HEADER_FONT_COLOR       = QColor("#699BA2"); // clazy:exclude=non-pod-global-static
const QColor      CELL_BACKGROUND_COLOR   = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      CELL_FONT_COLOR         = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr qint64 MS_IN_SECOND          = 1000LL;
constexpr qint64 ONE_MINUTE            = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR              = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY               = 24LL * ONE_HOUR;
constexpr qint64 TOOLTIP_HIDE_DELAY    = MS_IN_SECOND; // 1 second
constexpr double ZOOM_FACTOR_BASE      = 1.001;
constexpr double CHART_PEN_SIZE_FACTOR = 3000.0;
constexpr double CHART_PEN_MAX_SIZE    = 3.0;
constexpr int    LIMIT_AMOUNT_OF_DAYS  = 90;
constexpr int    MONTH_COUNT           = 12;
constexpr int    TITLE_FONT_SIZE       = 16;
constexpr qreal  TOOLTIP_Z_VALUE       = 11;
constexpr double COLUMN_GAP            = 0.71;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static QList<TimeRange> TIME_RANGES = {TIME_RANGE_LAST_DAY, TIME_RANGE_LAST_WEEK, TIME_RANGE_LAST_MONTH, TIME_RANGE_LAST_3_MONTH};
const qint64            TIME_RANGES_DELTAS[TIME_RANGE_COUNT] = {0, ONE_DAY, 7 * ONE_DAY, 31 * ONE_DAY, 90 * ONE_DAY};



AccountChartWidget::AccountChartWidget(IFileDialogFactory* fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent) :
    IAccountChartWidget(parent),
    tooltipHideTimer(),
    tooltip(),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor),
    mChartType(),
    mTimeRange(),
    mYieldChart(),
    mYieldSeries(),
    mYieldAxisX(),
    mYieldAxisY(),
    mMonthlyYieldChart(),
    mMonthlyYieldSeries(),
    mMonthlyYieldPositiveBarSet("Positive"),
    mMonthlyYieldNegativeBarSet("Negative"),
    mMonthlyYieldPositivePoints(),
    mMonthlyYieldNegativePoints(),
    mMonthlyYieldAxisX(),
    mMonthlyYieldAxisY(),
    mDailyYieldChart(),
    mDailyYieldSeries(),
    mDailyYieldPositiveBarSet("Positive"),
    mDailyYieldNegativeBarSet("Negative"),
    mDailyYieldPositivePoints(),
    mDailyYieldNegativePoints(),
    mDailyYieldAxisX(),
    mDailyYieldAxisY(),
    mRemainedMoneyChart(),
    mRemainedMoneySeries(),
    mRemainedMoneyAxisX(),
    mRemainedMoneyAxisY(),
    mTotalMoneyChart(),
    mTotalMoneySeries(),
    mTotalMoneyAxisX(),
    mTotalMoneyAxisY(),
    mMonthNames(),
    mLastMonthLimitsStart(),
    mLastMonthLimitsEnd(),
    mLastMonthlyYield(),
    mLastDayLimitsStart(),
    mLastDayLimitsEnd(),
    mLastDailyYield(),
    mAxisXMin(),
    mAxisXMax(),
    mYieldAxisYMin(),
    mYieldAxisYMax(),
    mMonthlyYieldAxisYMin(),
    mMonthlyYieldAxisYMax(),
    mDailyYieldAxisYMin(),
    mDailyYieldAxisYMax(),
    mRemainedMoneyAxisYMin(),
    mRemainedMoneyAxisYMax(),
    mTotalMoneyAxisYMin(),
    mTotalMoneyAxisYMax(),
    mTargetScenePos(),
    mTargetViewportPos()
{
    qDebug() << "Create AccountChartWidget";

    mMonthNames << tr("Jan") << tr("Feb") << tr("Mar") << tr("Apr") << tr("May") << tr("Jun") << tr("Jul") << tr("Aug")
                << tr("Sep") << tr("Oct") << tr("Nov") << tr("Dec");

    initYieldChart();
    initMonthlyYieldChart();
    initDailyYieldChart();
    initRemainedMoneyChart();
    initTotalMoneyChart();

    setChart(&mYieldChart);
    setRenderHint(QPainter::Antialiasing);

    setDragMode(DragMode::ScrollHandDrag);

    viewport()->installEventFilter(this);
    setMouseTracking(true);

    setContextMenuPolicy(Qt::CustomContextMenu);

    // clang-format off
    connect(this,                  SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));
    connect(&mYieldSeries,         SIGNAL(hovered(QPointF, bool)),             this, SLOT(lineSeriesHovered(QPointF, bool)));
    connect(&mMonthlyYieldSeries,  SIGNAL(hovered(bool, int, QBarSet*)),       this, SLOT(barSeriesHovered(bool, int, QBarSet*)));
    connect(&mDailyYieldSeries,    SIGNAL(hovered(bool, int, QBarSet*)),       this, SLOT(barSeriesHovered(bool, int, QBarSet*)));
    connect(&mRemainedMoneySeries, SIGNAL(hovered(QPointF, bool)),             this, SLOT(lineSeriesHovered(QPointF, bool)));
    connect(&mTotalMoneySeries,    SIGNAL(hovered(QPointF, bool)),             this, SLOT(lineSeriesHovered(QPointF, bool)));
    connect(&tooltipHideTimer,     SIGNAL(timeout()),                          this, SLOT(tooltipHideTimerTicked()));
    // clang-format on
}

AccountChartWidget::~AccountChartWidget()
{
    qDebug() << "Destroy AccountChartWidget";
}

void AccountChartWidget::zoom(double factor)
{
    scale(factor, factor);
    centerOn(mTargetScenePos);

    // NOLINTNEXTLINE(readability-magic-numbers)
    const QPointF deltaViewportPos = mTargetViewportPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
    const QPointF viewportCenter   = mapFromScene(mTargetScenePos) - deltaViewportPos;

    centerOn(mapToScene(viewportCenter.toPoint()));
}

bool AccountChartWidget::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

        mTargetViewportPos = mouseEvent->pos();
        mTargetScenePos    = mapToScene(mouseEvent->pos());
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = dynamic_cast<QWheelEvent*>(event);

        const double angle  = wheelEvent->angleDelta().y();
        const double factor = qPow(ZOOM_FACTOR_BASE, angle);

        zoom(factor);

        return true;
    }

    return IAccountChartWidget::eventFilter(object, event);
}

void AccountChartWidget::initYieldChart()
{
    mYieldChart.setTitle(tr("Yield"));
    mYieldChart.addSeries(&mYieldSeries);

    mYieldAxisX.setFormat(DATETIME_FORMAT);
    mYieldAxisX.setTitleText(tr("Time"));
    mYieldAxisY.setLabelFormat("%g");
    mYieldAxisY.setTitleText("%");

    mYieldChart.addAxis(&mYieldAxisX, Qt::AlignBottom);
    mYieldChart.addAxis(&mYieldAxisY, Qt::AlignLeft);
    mYieldSeries.attachAxis(&mYieldAxisX);
    mYieldSeries.attachAxis(&mYieldAxisY);

    initChartStyle(&mYieldChart, &mYieldAxisX, &mYieldAxisY);
}

void AccountChartWidget::initMonthlyYieldChart()
{
    mMonthlyYieldChart.setTitle(tr("Yield per month"));
    mMonthlyYieldChart.addSeries(&mMonthlyYieldSeries);

    mMonthlyYieldAxisX.setTitleText(tr("Time"));
    mMonthlyYieldAxisY.setLabelFormat("%g");
    mMonthlyYieldAxisY.setTitleText("%");

    mMonthlyYieldChart.addAxis(&mMonthlyYieldAxisX, Qt::AlignBottom);
    mMonthlyYieldChart.addAxis(&mMonthlyYieldAxisY, Qt::AlignLeft);
    mMonthlyYieldSeries.attachAxis(&mMonthlyYieldAxisX);
    mMonthlyYieldSeries.attachAxis(&mMonthlyYieldAxisY);

    mMonthlyYieldSeries.append(&mMonthlyYieldPositiveBarSet);
    mMonthlyYieldSeries.append(&mMonthlyYieldNegativeBarSet);

    mMonthlyYieldPositiveBarSet.setBrush(QBrush(GREEN_COLOR));
    mMonthlyYieldNegativeBarSet.setBrush(QBrush(RED_COLOR));
    mMonthlyYieldPositiveBarSet.setPen(QPen(Qt::NoPen));
    mMonthlyYieldNegativeBarSet.setPen(QPen(Qt::NoPen));

    mMonthlyYieldSeries.setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    mMonthlyYieldSeries.setLabelsFormat("@value %");
    mMonthlyYieldSeries.setLabelsPrecision(2);
    mMonthlyYieldSeries.setLabelsVisible(true);

    initChartStyle(&mMonthlyYieldChart, &mMonthlyYieldAxisX, &mMonthlyYieldAxisY);
}

void AccountChartWidget::initDailyYieldChart()
{
    mDailyYieldChart.setTitle(tr("Yield per day"));
    mDailyYieldChart.addSeries(&mDailyYieldSeries);

    mDailyYieldAxisX.setTitleText(tr("Time"));
    mDailyYieldAxisY.setLabelFormat("%g");
    mDailyYieldAxisY.setTitleText(tr("Money") + ", \u20BD");

    mDailyYieldChart.addAxis(&mDailyYieldAxisX, Qt::AlignBottom);
    mDailyYieldChart.addAxis(&mDailyYieldAxisY, Qt::AlignLeft);
    mDailyYieldSeries.attachAxis(&mDailyYieldAxisX);
    mDailyYieldSeries.attachAxis(&mDailyYieldAxisY);

    mDailyYieldSeries.append(&mDailyYieldPositiveBarSet);
    mDailyYieldSeries.append(&mDailyYieldNegativeBarSet);

    mDailyYieldPositiveBarSet.setBrush(QBrush(GREEN_COLOR));
    mDailyYieldNegativeBarSet.setBrush(QBrush(RED_COLOR));
    mDailyYieldPositiveBarSet.setPen(QPen(Qt::NoPen));
    mDailyYieldNegativeBarSet.setPen(QPen(Qt::NoPen));

    mDailyYieldSeries.setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    mDailyYieldSeries.setLabelsFormat("@value \u20BD");
    mDailyYieldSeries.setLabelsPrecision(2);
    mDailyYieldSeries.setLabelsVisible(true);

    initChartStyle(&mDailyYieldChart, &mDailyYieldAxisX, &mDailyYieldAxisY);
}

void AccountChartWidget::initRemainedMoneyChart()
{
    mRemainedMoneyChart.setTitle(tr("Remained money on account"));
    mRemainedMoneyChart.addSeries(&mRemainedMoneySeries);

    mRemainedMoneyAxisX.setFormat(DATETIME_FORMAT);
    mRemainedMoneyAxisX.setTitleText(tr("Time"));
    mRemainedMoneyAxisY.setLabelFormat("%g");
    mRemainedMoneyAxisY.setTitleText(tr("Money") + ", \u20BD");

    mRemainedMoneyChart.addAxis(&mRemainedMoneyAxisX, Qt::AlignBottom);
    mRemainedMoneyChart.addAxis(&mRemainedMoneyAxisY, Qt::AlignLeft);
    mRemainedMoneySeries.attachAxis(&mRemainedMoneyAxisX);
    mRemainedMoneySeries.attachAxis(&mRemainedMoneyAxisY);

    initChartStyle(&mRemainedMoneyChart, &mRemainedMoneyAxisX, &mRemainedMoneyAxisY);
}

void AccountChartWidget::initTotalMoneyChart()
{
    mTotalMoneyChart.setTitle(tr("Total money on account"));
    mTotalMoneyChart.addSeries(&mTotalMoneySeries);

    mTotalMoneyAxisX.setFormat(DATETIME_FORMAT);
    mTotalMoneyAxisX.setTitleText(tr("Time"));
    mTotalMoneyAxisY.setLabelFormat("%g");
    mTotalMoneyAxisY.setTitleText(tr("Money") + ", \u20BD");

    mTotalMoneyChart.addAxis(&mTotalMoneyAxisX, Qt::AlignBottom);
    mTotalMoneyChart.addAxis(&mTotalMoneyAxisY, Qt::AlignLeft);
    mTotalMoneySeries.attachAxis(&mTotalMoneyAxisX);
    mTotalMoneySeries.attachAxis(&mTotalMoneyAxisY);

    initChartStyle(&mTotalMoneyChart, &mTotalMoneyAxisX, &mTotalMoneyAxisY);
}

void AccountChartWidget::initChartStyle(QChart* chart, QAbstractAxis* axisX, QAbstractAxis* axisY)
{
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);

    chart->legend()->hide();
    chart->setAcceptHoverEvents(true);

    chart->setBackgroundBrush(QBrush(BACKGROUND_COLOR));
    chart->setPlotAreaBackgroundBrush(QBrush(PLOT_AREA_COLOR));
    chart->setPlotAreaBackgroundVisible(true);

    QFont font;
    font.setPixelSize(TITLE_FONT_SIZE);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(TITLE_COLOR));

    axisX->setLinePenColor(AXIS_COLOR);
    axisY->setLinePenColor(AXIS_COLOR);
    axisX->setGridLineColor(GRID_COLOR);
    axisY->setGridLineColor(GRID_COLOR);

    axisX->setLabelsColor(LABEL_COLOR);
    axisY->setLabelsColor(LABEL_COLOR);

    axisX->setTitleBrush(QBrush(TITLE_COLOR));
    axisY->setTitleBrush(QBrush(TITLE_COLOR));
}

void AccountChartWidget::switchChart(ChartType chartType)
{
    mChartType = chartType;

    switch (mChartType)
    {
        case CHART_TYPE_YIELD:
        {
            setChart(&mYieldChart);
            break;
        }
        case CHART_TYPE_MONTHLY_YIELD:
        {
            setChart(&mMonthlyYieldChart);
            break;
        }
        case CHART_TYPE_DAILY_YIELD:
        {
            setChart(&mDailyYieldChart);
            break;
        }
        case CHART_TYPE_REMAINED_MONEY:
        {
            setChart(&mRemainedMoneyChart);
            break;
        }
        case CHART_TYPE_TOTAL_MONEY:
        {
            setChart(&mTotalMoneyChart);
            break;
        }
    }

    if (tooltip != nullptr)
    {
        tooltipHideTimer.stop();

        delete tooltip;
        tooltip = nullptr;
    }
}

void AccountChartWidget::setTimeRange(TimeRange range)
{
    mTimeRange = range;

    if (mYieldSeries.count() > 0)
    {
        mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mYieldAxisY.setRange(mYieldAxisYMin[mTimeRange], mYieldAxisYMax[mTimeRange]);
        mRemainedMoneyAxisX.setRange(
            QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax)
        );
        mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin[mTimeRange], mRemainedMoneyAxisYMax[mTimeRange]);
        mTotalMoneyAxisX.setRange(
            QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax)
        );
        mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin[mTimeRange], mTotalMoneyAxisYMax[mTimeRange]);
    }
    else
    {
        mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
        mYieldAxisY.setRange(0, 0);
        mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
        mRemainedMoneyAxisY.setRange(0, 0);
        mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
        mTotalMoneyAxisY.setRange(0, 0);
    }
}

void AccountChartWidget::operationsRead(const QList<Operation>& operations)
{
    mYieldSeries.clear();
    mMonthlyYieldPositiveBarSet.remove(0, mMonthlyYieldPositiveBarSet.count());
    mMonthlyYieldNegativeBarSet.remove(0, mMonthlyYieldNegativeBarSet.count());
    mMonthlyYieldPositivePoints.clear();
    mMonthlyYieldNegativePoints.clear();
    mDailyYieldPositiveBarSet.remove(0, mDailyYieldPositiveBarSet.count());
    mDailyYieldNegativeBarSet.remove(0, mDailyYieldNegativeBarSet.count());
    mDailyYieldPositivePoints.clear();
    mDailyYieldNegativePoints.clear();
    mRemainedMoneySeries.clear();
    mTotalMoneySeries.clear();

    mMonthlyYieldAxisX.clear();
    mDailyYieldAxisX.clear();

    mLastMonthLimitsStart = 0;
    mLastMonthLimitsEnd   = 0;
    mLastMonthlyYield     = 0.0f;
    mLastDayLimitsStart   = 0;
    mLastDayLimitsEnd     = 0;
    mLastDailyYield       = 0.0f;

    mAxisXMin[0]              = std::numeric_limits<qint64>::max();
    mAxisXMax                 = std::numeric_limits<qint64>::lowest();
    mYieldAxisYMin[0]         = std::numeric_limits<float>::max();
    mYieldAxisYMax[0]         = std::numeric_limits<float>::lowest();
    mMonthlyYieldAxisYMin     = std::numeric_limits<float>::max();
    mMonthlyYieldAxisYMax     = std::numeric_limits<float>::lowest();
    mDailyYieldAxisYMin       = std::numeric_limits<float>::max();
    mDailyYieldAxisYMax       = std::numeric_limits<float>::lowest();
    mRemainedMoneyAxisYMin[0] = 0.0f;
    mRemainedMoneyAxisYMax[0] = std::numeric_limits<float>::lowest();
    mTotalMoneyAxisYMin[0]    = 0.0f;
    mTotalMoneyAxisYMax[0]    = std::numeric_limits<float>::lowest();

    if (!operations.isEmpty())
    {
        QList<QPointF> yieldPoints;
        QList<QPointF> remainedMoneyPoints;
        QList<QPointF> totalMoneyPoints;

        yieldPoints.reserve(operations.size());
        remainedMoneyPoints.reserve(operations.size());
        totalMoneyPoints.reserve(operations.size());

        for (int i = operations.size() - 1; i >= 0; --i)
        {
            handleOperation(operations.at(i), yieldPoints, remainedMoneyPoints, totalMoneyPoints);
        }

        mYieldSeries.replace(yieldPoints);
        syncBarSetFromPoints(&mMonthlyYieldPositiveBarSet, mMonthlyYieldPositivePoints);
        syncBarSetFromPoints(&mMonthlyYieldNegativeBarSet, mMonthlyYieldNegativePoints);
        syncBarSetFromPoints(&mDailyYieldPositiveBarSet, mDailyYieldPositivePoints);
        syncBarSetFromPoints(&mDailyYieldNegativeBarSet, mDailyYieldNegativePoints);
        mRemainedMoneySeries.replace(remainedMoneyPoints);
        mTotalMoneySeries.replace(totalMoneyPoints);

        syncTimeRangeSeries();

        mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mYieldAxisY.setRange(mYieldAxisYMin[mTimeRange], mYieldAxisYMax[mTimeRange]);
        mMonthlyYieldAxisY.setRange(mMonthlyYieldAxisYMin, mMonthlyYieldAxisYMax);
        mDailyYieldAxisY.setRange(mDailyYieldAxisYMin, mDailyYieldAxisYMax);
        mRemainedMoneyAxisX.setRange(
            QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax)
        );
        mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin[mTimeRange], mRemainedMoneyAxisYMax[mTimeRange]);
        mTotalMoneyAxisX.setRange(
            QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax)
        );
        mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin[mTimeRange], mTotalMoneyAxisYMax[mTimeRange]);

        QPen pen(SERIES_COLOR);
        pen.setWidthF(qMin(CHART_PEN_SIZE_FACTOR / mYieldSeries.count(), CHART_PEN_MAX_SIZE));
        mYieldSeries.setPen(pen);
        mRemainedMoneySeries.setPen(pen);
        mTotalMoneySeries.setPen(pen);
    }
    else
    {
        mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
        mYieldAxisY.setRange(0, 0);
        mMonthlyYieldAxisY.setRange(0, 0);
        mDailyYieldAxisY.setRange(0, 0);
        mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
        mRemainedMoneyAxisY.setRange(0, 0);
        mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
        mTotalMoneyAxisY.setRange(0, 0);
    }

    scene()->invalidate();
}

void AccountChartWidget::operationsAdded(const QList<Operation>& operations)
{
    QList<QPointF> yieldPoints;
    QList<QPointF> remainedMoneyPoints;
    QList<QPointF> totalMoneyPoints;

    yieldPoints.reserve(operations.size());
    remainedMoneyPoints.reserve(operations.size());
    totalMoneyPoints.reserve(operations.size());

    for (int i = operations.size() - 1; i >= 0; --i)
    {
        handleOperation(operations.at(i), yieldPoints, remainedMoneyPoints, totalMoneyPoints);
    }

    mYieldSeries.append(yieldPoints);
    syncBarSetFromPoints(&mMonthlyYieldPositiveBarSet, mMonthlyYieldPositivePoints);
    syncBarSetFromPoints(&mMonthlyYieldNegativeBarSet, mMonthlyYieldNegativePoints);
    syncBarSetFromPoints(&mDailyYieldPositiveBarSet, mDailyYieldPositivePoints);
    syncBarSetFromPoints(&mDailyYieldNegativeBarSet, mDailyYieldNegativePoints);
    mRemainedMoneySeries.append(remainedMoneyPoints);
    mTotalMoneySeries.append(totalMoneyPoints);

    syncTimeRangeSeries();

    mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mYieldAxisY.setRange(mYieldAxisYMin[mTimeRange], mYieldAxisYMax[mTimeRange]);
    mMonthlyYieldAxisY.setRange(mMonthlyYieldAxisYMin, mMonthlyYieldAxisYMax);
    mDailyYieldAxisY.setRange(mDailyYieldAxisYMin, mDailyYieldAxisYMax);
    mRemainedMoneyAxisX.setRange(
        QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax)
    );
    mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin[mTimeRange], mRemainedMoneyAxisYMax[mTimeRange]);
    mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin[mTimeRange]), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin[mTimeRange], mTotalMoneyAxisYMax[mTimeRange]);

    QPen pen(SERIES_COLOR);
    pen.setWidthF(qMin(CHART_PEN_SIZE_FACTOR / mYieldSeries.count(), CHART_PEN_MAX_SIZE));
    mYieldSeries.setPen(pen);
    mRemainedMoneySeries.setPen(pen);
    mTotalMoneySeries.setPen(pen);

    scene()->invalidate();
}

void AccountChartWidget::handleOperation(
    const Operation& operation, QList<QPointF>& yieldPoints, QList<QPointF>& remainedMoneyPoints, QList<QPointF>& totalMoneyPoints
)
{
    const float yieldPercent  = operation.totalYieldWithCommissionPercent;
    const float yield         = quotationToFloat(operation.totalYieldWithCommission);
    const float remainedMoney = quotationToFloat(operation.remainedMoney);
    const float totalMoney    = quotationToFloat(operation.totalMoney);

    if (operation.timestamp < mLastMonthLimitsStart || operation.timestamp > mLastMonthLimitsEnd)
    {
        const QDate operationDate = QDateTime::fromMSecsSinceEpoch(operation.timestamp).date();

        const int year  = operationDate.year();
        const int month = operationDate.month();

        mLastMonthLimitsStart = QDateTime(QDate(year, month, 1), QTime(0, 0)).toMSecsSinceEpoch();
        mLastMonthLimitsEnd =
            QDateTime(QDate(month == MONTH_COUNT ? year + 1 : year, month == MONTH_COUNT ? 1 : month + 1, 1), QTime(0, 0))
                .toMSecsSinceEpoch();

        mLastMonthlyYield += mMonthlyYieldPositivePoints.count() > 0
                                 ? mMonthlyYieldPositivePoints.at(mMonthlyYieldPositivePoints.count() - 1) +
                                       mMonthlyYieldNegativePoints.at(mMonthlyYieldNegativePoints.count() - 1)
                                 : 0.0f;

        mMonthlyYieldAxisX.append(QString("%1 %2").arg(mMonthNames.at(month - 1), QString::number(year)));
        mMonthlyYieldPositivePoints.append(0);
        mMonthlyYieldNegativePoints.append(0);
    }

    if (operation.timestamp < mLastDayLimitsStart || operation.timestamp > mLastDayLimitsEnd)
    {
        const QDate operationDate = QDateTime::fromMSecsSinceEpoch(operation.timestamp).date();

        mLastDayLimitsStart = QDateTime(operationDate, QTime(0, 0)).toMSecsSinceEpoch();
        mLastDayLimitsEnd   = mLastDayLimitsStart + ONE_DAY;

        mLastDailyYield += mDailyYieldPositivePoints.count() > 0
                               ? mDailyYieldPositivePoints.at(mDailyYieldPositivePoints.count() - 1) +
                                     mDailyYieldNegativePoints.at(mDailyYieldNegativePoints.count() - 1)
                               : 0.0f;

        if (mDailyYieldAxisX.count() >= LIMIT_AMOUNT_OF_DAYS)
        {
            mDailyYieldAxisX.remove(mDailyYieldAxisX.at(0));
            mDailyYieldPositivePoints.remove(0);
            mDailyYieldNegativePoints.remove(0);
        }

        mDailyYieldAxisX.append(operationDate.toString(DATE_FORMAT));
        mDailyYieldPositivePoints.append(0);
        mDailyYieldNegativePoints.append(0);
    }

    const float monthlyYield = yieldPercent - mLastMonthlyYield;
    const float dailyYield   = yield - mLastDailyYield;

    mAxisXMin[0] = qMin(mAxisXMin[0], operation.timestamp);
    mAxisXMax    = qMax(mAxisXMax, operation.timestamp);

    mYieldAxisYMin[0]         = qMin(mYieldAxisYMin[0], yieldPercent);
    mYieldAxisYMax[0]         = qMax(mYieldAxisYMax[0], yieldPercent);
    mMonthlyYieldAxisYMin     = qMin(mMonthlyYieldAxisYMin, monthlyYield);
    mMonthlyYieldAxisYMax     = qMax(mMonthlyYieldAxisYMax, monthlyYield);
    mDailyYieldAxisYMin       = qMin(mDailyYieldAxisYMin, dailyYield);
    mDailyYieldAxisYMax       = qMax(mDailyYieldAxisYMax, dailyYield);
    mRemainedMoneyAxisYMin[0] = qMin(mRemainedMoneyAxisYMin[0], remainedMoney);
    mRemainedMoneyAxisYMax[0] = qMax(mRemainedMoneyAxisYMax[0], remainedMoney);
    mTotalMoneyAxisYMin[0]    = qMin(mTotalMoneyAxisYMin[0], totalMoney);
    mTotalMoneyAxisYMax[0]    = qMax(mTotalMoneyAxisYMax[0], totalMoney);

    yieldPoints.append(QPointF(operation.timestamp, yieldPercent));
    mMonthlyYieldPositivePoints.replace(mMonthlyYieldPositivePoints.count() - 1, qMax(monthlyYield, 0.0f));
    mMonthlyYieldNegativePoints.replace(mMonthlyYieldNegativePoints.count() - 1, qMin(monthlyYield, 0.0f));
    mDailyYieldPositivePoints.replace(mDailyYieldPositivePoints.count() - 1, qMax(dailyYield, 0.0f));
    mDailyYieldNegativePoints.replace(mDailyYieldNegativePoints.count() - 1, qMin(dailyYield, 0.0f));
    remainedMoneyPoints.append(QPointF(operation.timestamp, remainedMoney));
    totalMoneyPoints.append(QPointF(operation.timestamp, totalMoney));
}

struct SyncTimeRangeSeriesInfo
{
    explicit SyncTimeRangeSeriesInfo(AccountChartWidget* _thread) :
        thread(_thread)
    {
    }

    AccountChartWidget* thread;
};

static void syncTimeRangeSeriesForParallel(
    QThread* parentThread,
    int /*threadId*/,
    TimeRange* timeRanges, // NOLINT(readability-non-const-parameter)
    int /*size*/,
    int   start,
    int   end,
    void* additionalArgs
)
{
    SyncTimeRangeSeriesInfo* syncTimeRangeSeriesInfo = reinterpret_cast<SyncTimeRangeSeriesInfo*>(additionalArgs);

    AccountChartWidget* thread = syncTimeRangeSeriesInfo->thread;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const TimeRange timeRange = timeRanges[i];

        thread->syncYieldTimeRangeSeries(timeRange);
        thread->syncRemainedMoneyTimeRangeSeries(timeRange);
        thread->syncTotalMoneyTimeRangeSeries(timeRange);
    }
}

void AccountChartWidget::syncTimeRangeSeries()
{
    SyncTimeRangeSeriesInfo syncTimeRangeSeriesInfo(this);
    processInParallel(QThread::currentThread(), TIME_RANGES, syncTimeRangeSeriesForParallel, &syncTimeRangeSeriesInfo);
}

void AccountChartWidget::syncYieldTimeRangeSeries(TimeRange timeRange)
{
    const QList<QPointF>& yieldSeriesPoints = mYieldSeries.points();

    const int index = std::distance(
        yieldSeriesPoints.constBegin(),
        std::lower_bound(
            yieldSeriesPoints.constBegin(),
            yieldSeriesPoints.constEnd(),
            yieldSeriesPoints.constLast().x() - TIME_RANGES_DELTAS[timeRange],
            [](const QPointF& point, qint64 value) { return point.x() < value; }
        )
    );

    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    for (int i = index; i < yieldSeriesPoints.size(); ++i)
    {
        const qreal y = yieldSeriesPoints.at(i).y();

        minY = qMin(y, minY);
        maxY = qMax(y, maxY);
    }

    mAxisXMin[timeRange]      = yieldSeriesPoints.at(index).x();
    mYieldAxisYMin[timeRange] = minY;
    mYieldAxisYMax[timeRange] = maxY;
}

void AccountChartWidget::syncRemainedMoneyTimeRangeSeries(TimeRange timeRange)
{
    const QList<QPointF>& remainedMoneySeriesPoints = mRemainedMoneySeries.points();

    const int index = std::distance(
        remainedMoneySeriesPoints.constBegin(),
        std::lower_bound(
            remainedMoneySeriesPoints.constBegin(),
            remainedMoneySeriesPoints.constEnd(),
            remainedMoneySeriesPoints.constLast().x() - TIME_RANGES_DELTAS[timeRange],
            [](const QPointF& point, qint64 value) { return point.x() < value; }
        )
    );

    float minY = 0.0f;
    float maxY = std::numeric_limits<float>::lowest();

    for (int i = index; i < remainedMoneySeriesPoints.size(); ++i)
    {
        const qreal y = remainedMoneySeriesPoints.at(i).y();

        minY = qMin(y, minY);
        maxY = qMax(y, maxY);
    }

    mAxisXMin[timeRange]              = remainedMoneySeriesPoints.at(index).x();
    mRemainedMoneyAxisYMin[timeRange] = minY;
    mRemainedMoneyAxisYMax[timeRange] = maxY;
}

void AccountChartWidget::syncTotalMoneyTimeRangeSeries(TimeRange timeRange)
{
    const QList<QPointF>& totalMoneySeriesPoints = mTotalMoneySeries.points();

    const int index = std::distance(
        totalMoneySeriesPoints.constBegin(),
        std::lower_bound(
            totalMoneySeriesPoints.constBegin(),
            totalMoneySeriesPoints.constEnd(),
            totalMoneySeriesPoints.constLast().x() - TIME_RANGES_DELTAS[timeRange],
            [](const QPointF& point, qint64 value) { return point.x() < value; }
        )
    );

    float minY = 0.0f;
    float maxY = std::numeric_limits<float>::lowest();

    for (int i = index; i < totalMoneySeriesPoints.size(); ++i)
    {
        const qreal y = totalMoneySeriesPoints.at(i).y();

        minY = qMin(y, minY);
        maxY = qMax(y, maxY);
    }

    mAxisXMin[timeRange]           = totalMoneySeriesPoints.at(index).x();
    mTotalMoneyAxisYMin[timeRange] = minY;
    mTotalMoneyAxisYMax[timeRange] = maxY;
}

void AccountChartWidget::syncBarSetFromPoints(QBarSet* barSet, const QList<qreal>& points)
{
    Q_ASSERT_X(points.count() > 0, __FUNCTION__, "points is empty");
    Q_ASSERT_X(barSet->count() <= points.count(), __FUNCTION__, "barSet should be smaller than points");

    if (barSet->count() > 0)
    {
        const qreal barSetValue = barSet->at(barSet->count() - 1);
        const qreal pointValue  = points.at(barSet->count() - 1);

        if (barSetValue != pointValue)
        {
            barSet->replace(barSet->count() - 1, pointValue);
        }
    }

    for (int i = barSet->count(); i < points.count(); ++i)
    {
        barSet->append(points.at(i));
    }
}

void AccountChartWidget::contextMenuRequested(const QPoint& pos)
{
    QMenu* contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Export to Excel"), this, SLOT(actionExportToExcelTriggered()));

    contextMenu->popup(viewport()->mapToGlobal(pos));
}

void AccountChartWidget::actionExportToExcelTriggered()
{
    const QString lastFile = mSettingsEditor->value("MainWindow/AccountChartWidget/exportToExcelFile", "").toString();

    const std::shared_ptr<IFileDialog> fileDialog = mFileDialogFactory->newInstance(
        this, tr("Export"), lastFile.left(lastFile.lastIndexOf("/")), tr("Excel file") + " (*.xlsx)"
    );
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDefaultSuffix("xlsx");

    fileDialog->selectFile(lastFile);

    if (fileDialog->exec() == QDialog::Accepted)
    {
        const QString path = fileDialog->selectedFiles().at(0);
        mSettingsEditor->setValue("MainWindow/AccountChartWidget/exportToExcelFile", path);

        exportToExcel(path);
    }
}

void AccountChartWidget::exportToExcel(const QString& path) const
{
    QXlsx::Document doc;

    QXlsx::Format headerStyle;
    headerStyle.setFontBold(true);
    headerStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    headerStyle.setFillPattern(QXlsx::Format::PatternSolid);
    headerStyle.setBorderStyle(QXlsx::Format::BorderThin);
    headerStyle.setPatternBackgroundColor(HEADER_BACKGROUND_COLOR);
    headerStyle.setFontColor(HEADER_FONT_COLOR);

    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    QXlsx::Format dateFormat;
    dateFormat.setNumberFormat(DATETIME_FORMAT);
    dateFormat.setFillPattern(QXlsx::Format::PatternSolid);
    dateFormat.setBorderStyle(QXlsx::Format::BorderThin);
    dateFormat.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    dateFormat.setFontColor(CELL_FONT_COLOR);

    doc.addSheet(mYieldChart.title() + " (Data)");
    doc.write(1, 1, mYieldAxisX.titleText(), headerStyle);
    doc.write(1, 2, mYieldAxisY.titleText(), headerStyle);

    const QList<QPointF>& yieldSeriesPoints = mYieldSeries.points();

    for (int i = 0; i < yieldSeriesPoints.size(); ++i)
    {
        const QPointF& point = yieldSeriesPoints.at(i);

        doc.write(i + 2, 1, QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x())), dateFormat);
        doc.write(i + 2, 2, point.y(), cellStyle);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.setColumnWidth(1, 17.57 + COLUMN_GAP);
    doc.setColumnWidth(2, 12    + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.addSheet(mMonthlyYieldChart.title() + " (Data)");
    doc.write(1, 1, mMonthlyYieldAxisX.titleText(), headerStyle);
    doc.write(1, 2, mMonthlyYieldAxisY.titleText(), headerStyle);

    for (int i = 0; i < mMonthlyYieldPositiveBarSet.count(); ++i)
    {
        doc.write(i + 2, 1, mMonthlyYieldAxisX.at(i), cellStyle);
        doc.write(i + 2, 2, mMonthlyYieldPositiveBarSet.at(i) + mMonthlyYieldNegativeBarSet.at(i), cellStyle);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.setColumnWidth(1, 17.57 + COLUMN_GAP);
    doc.setColumnWidth(2, 12    + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.addSheet(mDailyYieldChart.title() + " (Data)");
    doc.write(1, 1, mDailyYieldAxisX.titleText(), headerStyle);
    doc.write(1, 2, mDailyYieldAxisY.titleText(), headerStyle);

    for (int i = 0; i < mDailyYieldPositiveBarSet.count(); ++i)
    {
        doc.write(i + 2, 1, mDailyYieldAxisX.at(i), cellStyle);
        doc.write(i + 2, 2, mDailyYieldPositiveBarSet.at(i) + mDailyYieldNegativeBarSet.at(i), cellStyle);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.setColumnWidth(1, 17.57 + COLUMN_GAP);
    doc.setColumnWidth(2, 12    + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.addSheet(mRemainedMoneyChart.title() + " (Data)");
    doc.write(1, 1, mRemainedMoneyAxisX.titleText(), headerStyle);
    doc.write(1, 2, mRemainedMoneyAxisY.titleText(), headerStyle);

    const QList<QPointF>& remainedMoneySeriesPoints = mRemainedMoneySeries.points();

    for (int i = 0; i < remainedMoneySeriesPoints.size(); ++i)
    {
        const QPointF& point = remainedMoneySeriesPoints.at(i);

        doc.write(i + 2, 1, QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x())), dateFormat);
        doc.write(i + 2, 2, point.y(), cellStyle);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.setColumnWidth(1, 17.57 + COLUMN_GAP);
    doc.setColumnWidth(2, 12    + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.addSheet(mTotalMoneyChart.title() + " (Data)");
    doc.write(1, 1, mTotalMoneyAxisX.titleText(), headerStyle);
    doc.write(1, 2, mTotalMoneyAxisY.titleText(), headerStyle);

    const QList<QPointF>& totalMoneyPoints = mTotalMoneySeries.points();

    for (int i = 0; i < totalMoneyPoints.size(); ++i)
    {
        const QPointF& point = totalMoneyPoints.at(i);

        doc.write(i + 2, 1, QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x())), dateFormat);
        doc.write(i + 2, 2, point.y(), cellStyle);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.setColumnWidth(1, 17.57 + COLUMN_GAP);
    doc.setColumnWidth(2, 12    + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.insertSheet(0, mTotalMoneyChart.title(), QXlsx::AbstractSheet::ST_ChartSheet);
    QXlsx::Chartsheet* totalMoneySheet = dynamic_cast<QXlsx::Chartsheet*>(doc.currentSheet());
    QXlsx::Chart*      totalMoneyChart = totalMoneySheet->chart();
    totalMoneyChart->setChartType(QXlsx::Chart::CT_LineChart);
    totalMoneyChart->setChartTitle(mTotalMoneyChart.title());
    totalMoneyChart->setAxisTitle(QXlsx::Chart::Bottom, mTotalMoneyAxisX.titleText());
    totalMoneyChart->setAxisTitle(QXlsx::Chart::Left, mTotalMoneyAxisY.titleText());
    totalMoneyChart->addSeries(
        QXlsx::CellRange(2, 1, totalMoneyPoints.size() + 1, 2), doc.sheet(mTotalMoneyChart.title() + " (Data)"), true
    );

    doc.insertSheet(0, mRemainedMoneyChart.title(), QXlsx::AbstractSheet::ST_ChartSheet);
    QXlsx::Chartsheet* remainedMoneySheet = dynamic_cast<QXlsx::Chartsheet*>(doc.currentSheet());
    QXlsx::Chart*      remainedMoneyChart = remainedMoneySheet->chart();
    remainedMoneyChart->setChartType(QXlsx::Chart::CT_LineChart);
    remainedMoneyChart->setChartTitle(mRemainedMoneyChart.title());
    remainedMoneyChart->setAxisTitle(QXlsx::Chart::Bottom, mRemainedMoneyAxisX.titleText());
    remainedMoneyChart->setAxisTitle(QXlsx::Chart::Left, mRemainedMoneyAxisY.titleText());
    remainedMoneyChart->addSeries(
        QXlsx::CellRange(2, 1, remainedMoneySeriesPoints.size() + 1, 2), doc.sheet(mRemainedMoneyChart.title() + " (Data)"), true
    );

    doc.insertSheet(0, mDailyYieldChart.title(), QXlsx::AbstractSheet::ST_ChartSheet);
    QXlsx::Chartsheet* dailyYieldSheet = dynamic_cast<QXlsx::Chartsheet*>(doc.currentSheet());
    QXlsx::Chart*      dailyYieldChart = dailyYieldSheet->chart();
    dailyYieldChart->setChartType(QXlsx::Chart::CT_BarChart);
    dailyYieldChart->setChartTitle(mDailyYieldChart.title());
    dailyYieldChart->setAxisTitle(QXlsx::Chart::Bottom, mDailyYieldAxisX.titleText());
    dailyYieldChart->setAxisTitle(QXlsx::Chart::Left, mDailyYieldAxisY.titleText());
    dailyYieldChart->addSeries(
        QXlsx::CellRange(2, 1, mDailyYieldPositiveBarSet.count() + 1, 2), doc.sheet(mDailyYieldChart.title() + " (Data)"), true
    );

    doc.insertSheet(0, mMonthlyYieldChart.title(), QXlsx::AbstractSheet::ST_ChartSheet);
    QXlsx::Chartsheet* monthlyYieldSheet = dynamic_cast<QXlsx::Chartsheet*>(doc.currentSheet());
    QXlsx::Chart*      monthlyYieldChart = monthlyYieldSheet->chart();
    monthlyYieldChart->setChartType(QXlsx::Chart::CT_BarChart);
    monthlyYieldChart->setChartTitle(mMonthlyYieldChart.title());
    monthlyYieldChart->setAxisTitle(QXlsx::Chart::Bottom, mMonthlyYieldAxisX.titleText());
    monthlyYieldChart->setAxisTitle(QXlsx::Chart::Left, mMonthlyYieldAxisY.titleText());
    monthlyYieldChart->addSeries(
        QXlsx::CellRange(2, 1, mMonthlyYieldPositiveBarSet.count() + 1, 2),
        doc.sheet(mMonthlyYieldChart.title() + " (Data)"),
        true
    );

    doc.insertSheet(0, mYieldChart.title(), QXlsx::AbstractSheet::ST_ChartSheet);
    QXlsx::Chartsheet* yieldSheet = dynamic_cast<QXlsx::Chartsheet*>(doc.currentSheet());
    QXlsx::Chart*      yieldChart = yieldSheet->chart();
    yieldChart->setChartType(QXlsx::Chart::CT_LineChart);
    yieldChart->setChartTitle(mYieldChart.title());
    yieldChart->setAxisTitle(QXlsx::Chart::Bottom, mYieldAxisX.titleText());
    yieldChart->setAxisTitle(QXlsx::Chart::Left, mYieldAxisY.titleText());
    yieldChart->addSeries(
        QXlsx::CellRange(2, 1, yieldSeriesPoints.size() + 1, 2), doc.sheet(mYieldChart.title() + " (Data)"), true
    );

    doc.saveAs(path);
}

void AccountChartWidget::lineSeriesHovered(QPointF point, bool state)
{
    if (tooltip == nullptr)
    {
        tooltip = new ChartTooltip(chart());
        tooltip->setZValue(TOOLTIP_Z_VALUE);
    }

    if (state)
    {
        tooltipHideTimer.stop();

        QLineSeries* series = mChartType == CHART_TYPE_YIELD            ? &mYieldSeries
                              : mChartType == CHART_TYPE_REMAINED_MONEY ? &mRemainedMoneySeries
                                                                        : &mTotalMoneySeries;

        const QPointF nearestPoint = findNearestPoint(point, series->points());

        const QString prefix =
            (mChartType == CHART_TYPE_YIELD || mChartType == CHART_TYPE_MONTHLY_YIELD || mChartType == CHART_TYPE_DAILY_YIELD) &&
                    nearestPoint.y() > 0
                ? "+"
                : "";
        const QString suffix = mChartType == CHART_TYPE_YIELD || mChartType == CHART_TYPE_MONTHLY_YIELD ? "%" : " \u20BD";
        const QString xDescription =
            QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(nearestPoint.x())).toString(DATETIME_FORMAT);

        tooltip->setText(QString("%1\n%2%3%4").arg(xDescription, prefix, QString::number(nearestPoint.y(), 'f', 2), suffix));
        tooltip->setAnchor(nearestPoint);
        tooltip->updateGeometry();
        tooltip->show();
    }
    else
    {
        tooltipHideTimer.start(TOOLTIP_HIDE_DELAY);
    }
}

void AccountChartWidget::barSeriesHovered(bool status, int index, QBarSet* barSet)
{
    if (tooltip == nullptr)
    {
        tooltip = new ChartTooltip(chart());
        tooltip->setZValue(TOOLTIP_Z_VALUE);
    }

    if (status)
    {
        tooltipHideTimer.stop();

        const qreal   value        = barSet->at(index);
        const QPointF nearestPoint = QPointF(index, qMax(value, 0.0f));

        const QString prefix = value > 0 ? "+" : "";
        const QString suffix = mChartType == CHART_TYPE_MONTHLY_YIELD ? "%" : " \u20BD";
        const QString xDescription =
            mChartType == CHART_TYPE_MONTHLY_YIELD ? mMonthlyYieldAxisX.at(index) : mDailyYieldAxisX.at(index);

        tooltip->setText(QString("%1\n%2%3 %4").arg(xDescription, prefix, QString::number(value, 'f', 2), suffix));
        tooltip->setAnchor(nearestPoint);
        tooltip->updateGeometry();
        tooltip->show();
    }
    else
    {
        tooltipHideTimer.start(TOOLTIP_HIDE_DELAY);
    }
}

void AccountChartWidget::tooltipHideTimerTicked()
{
    tooltipHideTimer.stop();
    tooltip->hide();
}

QPointF AccountChartWidget::findNearestPoint(const QPointF& point, const QList<QPointF>& seriesPoints)
{
    Q_ASSERT_X(
        std::is_sorted(
            seriesPoints.constBegin(), seriesPoints.constEnd(), [](const QPointF& l, const QPointF& r) { return l.x() < r.x(); }
        ),
        __FUNCTION__,
        "Series points is unsorted"
    );

    const int index = std::distance(
        seriesPoints.constBegin(),
        std::lower_bound(
            seriesPoints.constBegin(), seriesPoints.constEnd(), point.x(), [](const QPointF& seriesPoint, qreal value) {
                return seriesPoint.x() < value;
            }
        )
    );

    if (index <= 0)
    {
        return seriesPoints.constFirst();
    }

    if (index >= seriesPoints.size())
    {
        return seriesPoints.constLast();
    }

    const qreal distance     = seriesPoints.at(index).x() - point.x();
    const qreal prevDistance = point.x() - seriesPoints.at(index - 1).x();

    if (distance < prevDistance)
    {
        return seriesPoints.at(index);
    }

    return seriesPoints.at(index - 1);
}
