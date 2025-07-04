#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>
#include <QGraphicsLayout>
#include <QMenu>

#include "src/grpc/utils.h"
#include "src/qxlsx/xlsxchart.h"
#include "src/qxlsx/xlsxchartsheet.h"
#include "src/qxlsx/xlsxdocument.h"



const char* const DATETIME_FORMAT         = "yyyy-MM-dd hh:mm:ss";
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
constexpr qint64 TOOLTIP_HIDE_DELAY    = MS_IN_SECOND; // 1 second
constexpr double ZOOM_FACTOR_BASE      = 1.001;
constexpr double CHART_PEN_SIZE_FACTOR = 3000.0;
constexpr double CHART_PEN_MAX_SIZE    = 3.0;
constexpr int    MONTH_COUNT           = 12;
constexpr int    TITLE_FONT_SIZE       = 16;
constexpr qreal  TOOLTIP_Z_VALUE       = 11;
constexpr double COLUMN_GAP            = 0.71;



AccountChartWidget::AccountChartWidget(IFileDialogFactory* fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent) :
    IAccountChartWidget(parent),
    tooltipHideTimer(),
    tooltip(),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor),
    mChartType(),
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
    mAxisXMin(),
    mAxisXMax(),
    mYieldAxisYMin(),
    mYieldAxisYMax(),
    mMonthlyYieldAxisYMin(),
    mMonthlyYieldAxisYMax(),
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

void AccountChartWidget::operationsRead(const QList<Operation>& operations)
{
    mYieldSeries.clear();
    mMonthlyYieldPositiveBarSet.remove(0, mMonthlyYieldPositiveBarSet.count());
    mMonthlyYieldNegativeBarSet.remove(0, mMonthlyYieldNegativeBarSet.count());
    mMonthlyYieldPositivePoints.clear();
    mMonthlyYieldNegativePoints.clear();
    mRemainedMoneySeries.clear();
    mTotalMoneySeries.clear();

    mMonthlyYieldAxisX.clear();

    mLastMonthLimitsStart = 0;
    mLastMonthLimitsEnd   = 0;
    mLastMonthlyYield     = 0.0f;

    mAxisXMin              = std::numeric_limits<qint64>::max();
    mAxisXMax              = std::numeric_limits<qint64>::min();
    mYieldAxisYMin         = std::numeric_limits<float>::max();
    mYieldAxisYMax         = std::numeric_limits<float>::min();
    mMonthlyYieldAxisYMin  = std::numeric_limits<float>::max();
    mMonthlyYieldAxisYMax  = std::numeric_limits<float>::min();
    mRemainedMoneyAxisYMin = 0.0f;
    mRemainedMoneyAxisYMax = std::numeric_limits<float>::min();
    mTotalMoneyAxisYMin    = 0.0f;
    mTotalMoneyAxisYMax    = std::numeric_limits<float>::min();

    if (!operations.isEmpty())
    {
        QList<QPointF> yieldPoints;
        QList<QPointF> remainedMoneyPoints;
        QList<QPointF> totalMoneySeriesPoints;

        yieldPoints.reserve(operations.size());
        remainedMoneyPoints.reserve(operations.size());
        totalMoneySeriesPoints.reserve(operations.size());

        for (int i = operations.size() - 1; i >= 0; --i)
        {
            handleOperation(operations.at(i), yieldPoints, remainedMoneyPoints, totalMoneySeriesPoints);
        }

        mYieldSeries.replace(yieldPoints);
        syncBarSetFromPoints(&mMonthlyYieldPositiveBarSet, mMonthlyYieldPositivePoints);
        syncBarSetFromPoints(&mMonthlyYieldNegativeBarSet, mMonthlyYieldNegativePoints);
        mRemainedMoneySeries.replace(remainedMoneyPoints);
        mTotalMoneySeries.replace(totalMoneySeriesPoints);

        mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mYieldAxisY.setRange(mYieldAxisYMin, mYieldAxisYMax);
        mMonthlyYieldAxisY.setRange(mMonthlyYieldAxisYMin, mMonthlyYieldAxisYMax);
        mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin, mRemainedMoneyAxisYMax);
        mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin, mTotalMoneyAxisYMax);

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
    QList<QPointF> totalMoneySeriesPoints;

    yieldPoints.reserve(operations.size());
    remainedMoneyPoints.reserve(operations.size());
    totalMoneySeriesPoints.reserve(operations.size());

    for (int i = operations.size() - 1; i >= 0; --i)
    {
        handleOperation(operations.at(i), yieldPoints, remainedMoneyPoints, totalMoneySeriesPoints);
    }

    mYieldSeries.append(yieldPoints);
    syncBarSetFromPoints(&mMonthlyYieldPositiveBarSet, mMonthlyYieldPositivePoints);
    syncBarSetFromPoints(&mMonthlyYieldNegativeBarSet, mMonthlyYieldNegativePoints);
    mRemainedMoneySeries.append(remainedMoneyPoints);
    mTotalMoneySeries.append(totalMoneySeriesPoints);

    mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mYieldAxisY.setRange(mYieldAxisYMin, mYieldAxisYMax);
    mMonthlyYieldAxisY.setRange(mMonthlyYieldAxisYMin, mMonthlyYieldAxisYMax);
    mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin, mRemainedMoneyAxisYMax);
    mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin, mTotalMoneyAxisYMax);

    QPen pen(SERIES_COLOR);
    pen.setWidthF(qMin(CHART_PEN_SIZE_FACTOR / mYieldSeries.count(), CHART_PEN_MAX_SIZE));
    mYieldSeries.setPen(pen);
    mRemainedMoneySeries.setPen(pen);
    mTotalMoneySeries.setPen(pen);

    scene()->invalidate();
}

void AccountChartWidget::handleOperation(
    const Operation& operation,
    QList<QPointF>&  yieldPoints,
    QList<QPointF>&  remainedMoneyPoints,
    QList<QPointF>&  totalMoneySeriesPoints
)
{
    const float yield         = operation.totalYieldWithCommissionPercent;
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

    const float monthlyYield = yield - mLastMonthlyYield;

    mAxisXMin = qMin(mAxisXMin, operation.timestamp);
    mAxisXMax = qMax(mAxisXMax, operation.timestamp);

    mYieldAxisYMin         = qMin(mYieldAxisYMin, yield);
    mYieldAxisYMax         = qMax(mYieldAxisYMax, yield);
    mMonthlyYieldAxisYMin  = qMin(mMonthlyYieldAxisYMin, monthlyYield);
    mMonthlyYieldAxisYMax  = qMax(mMonthlyYieldAxisYMax, monthlyYield);
    mRemainedMoneyAxisYMin = qMin(mRemainedMoneyAxisYMin, remainedMoney);
    mRemainedMoneyAxisYMax = qMax(mRemainedMoneyAxisYMax, remainedMoney);
    mTotalMoneyAxisYMin    = qMin(mTotalMoneyAxisYMin, totalMoney);
    mTotalMoneyAxisYMax    = qMax(mTotalMoneyAxisYMax, totalMoney);

    yieldPoints.append(QPointF(operation.timestamp, yield));
    mMonthlyYieldPositivePoints.replace(mMonthlyYieldPositivePoints.count() - 1, qMax(monthlyYield, 0.0f));
    mMonthlyYieldNegativePoints.replace(mMonthlyYieldNegativePoints.count() - 1, qMin(monthlyYield, 0.0f));
    remainedMoneyPoints.append(QPointF(operation.timestamp, remainedMoney));
    totalMoneySeriesPoints.append(QPointF(operation.timestamp, totalMoney));
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

    const QList<QPointF>& totalMoneySeriesPoints = mTotalMoneySeries.points();

    for (int i = 0; i < totalMoneySeriesPoints.size(); ++i)
    {
        const QPointF& point = totalMoneySeriesPoints.at(i);

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
        QXlsx::CellRange(2, 1, totalMoneySeriesPoints.size() + 1, 2), doc.sheet(mTotalMoneyChart.title() + " (Data)"), true
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
            (mChartType == CHART_TYPE_YIELD || mChartType == CHART_TYPE_MONTHLY_YIELD) && nearestPoint.y() > 0 ? "+" : "";
        const QString suffix = mChartType == CHART_TYPE_YIELD || mChartType == CHART_TYPE_MONTHLY_YIELD ? "%" : "\u20BD";
        const QString xDescription =
            QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(nearestPoint.x())).toString(DATETIME_FORMAT);

        tooltip->setText(QString("%1\n%2%3 %4").arg(xDescription, prefix, QString::number(nearestPoint.y(), 'f', 2), suffix));
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

        const QString prefix       = value > 0 ? "+" : "";
        const QString suffix       = "%";
        const QString xDescription = mMonthlyYieldAxisX.at(index);

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
