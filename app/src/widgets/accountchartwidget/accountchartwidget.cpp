#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>
#include <QGraphicsLayout>
#include <QMenu>

#include "src/grpc/utils.h"



const char* const DATETIME_FORMAT    = "yyyy-MM-dd hh:mm:ss";
const QColor      BACKGROUND_COLOR   = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      PLOT_AREA_COLOR    = QColor("#344759"); // clazy:exclude=non-pod-global-static
const QColor      TITLE_COLOR        = QColor("#FFFFFF"); // clazy:exclude=non-pod-global-static
const QColor      LABEL_COLOR        = QColor("#AFC2D7"); // clazy:exclude=non-pod-global-static
const QColor      AXIS_COLOR         = QColor("#FFFFFF"); // clazy:exclude=non-pod-global-static
const QColor      GRID_COLOR         = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      SERIES_COLOR       = QColor("#6D85FF"); // clazy:exclude=non-pod-global-static
constexpr qint64  MS_IN_SECOND       = 1000LL;
constexpr qint64  TOOLTIP_HIDE_DELAY = MS_IN_SECOND; // 1 second
constexpr double  ZOOM_FACTOR_BASE   = 1.001;
constexpr int     TITLE_FONT_SIZE    = 16;
constexpr qreal   TOOLTIP_Z_VALUE    = 11;



AccountChartWidget::AccountChartWidget(IFileDialogFactory* fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent) :
    IAccountChartWidget(parent),
    tooltipHideTimer(),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor),
    mChartType(),
    mYieldChart(),
    mYieldSeries(),
    mYieldAxisX(),
    mYieldAxisY(),
    mMonthlyYieldChart(),
    mMonthlyYieldSeries(),
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
    mTargetViewportPos(),
    mTooltip()
{
    qDebug() << "Create AccountChartWidget";

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
    connect(&mYieldSeries,         SIGNAL(hovered(QPointF, bool)),             this, SLOT(seriesHovered(QPointF, bool)));
    connect(&mMonthlyYieldSeries,  SIGNAL(hovered(QPointF, bool)),             this, SLOT(seriesHovered(QPointF, bool)));
    connect(&mRemainedMoneySeries, SIGNAL(hovered(QPointF, bool)),             this, SLOT(seriesHovered(QPointF, bool)));
    connect(&mTotalMoneySeries,    SIGNAL(hovered(QPointF, bool)),             this, SLOT(seriesHovered(QPointF, bool)));
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

    QPen pen(SERIES_COLOR);
    pen.setWidth(3);
    mYieldSeries.setPen(pen);

    initChartStyle(&mYieldChart, &mYieldAxisX, &mYieldAxisY);
}

void AccountChartWidget::initMonthlyYieldChart()
{
    mMonthlyYieldChart.setTitle(tr("Yield per month"));
    mMonthlyYieldChart.addSeries(&mMonthlyYieldSeries);

    mMonthlyYieldAxisX.setFormat(DATETIME_FORMAT);
    mMonthlyYieldAxisX.setTitleText(tr("Time"));
    mMonthlyYieldAxisY.setLabelFormat("%g");
    mMonthlyYieldAxisY.setTitleText("%");

    mMonthlyYieldChart.addAxis(&mMonthlyYieldAxisX, Qt::AlignBottom);
    mMonthlyYieldChart.addAxis(&mMonthlyYieldAxisY, Qt::AlignLeft);
    mMonthlyYieldSeries.attachAxis(&mMonthlyYieldAxisX);
    mMonthlyYieldSeries.attachAxis(&mMonthlyYieldAxisY);

    QPen pen(SERIES_COLOR);
    pen.setWidth(3);
    mMonthlyYieldSeries.setPen(pen);

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

    QPen pen(SERIES_COLOR);
    pen.setWidth(3);
    mRemainedMoneySeries.setPen(pen);

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

    QPen pen(SERIES_COLOR);
    pen.setWidth(3);
    mTotalMoneySeries.setPen(pen);

    initChartStyle(&mTotalMoneyChart, &mTotalMoneyAxisX, &mTotalMoneyAxisY);
}

void AccountChartWidget::initChartStyle(QChart* chart, QAbstractAxis* axisX, QAbstractAxis* axisY)
{
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);

    chart->legend()->hide();
    chart->setAcceptHoverEvents(true);
    chart->setAnimationOptions(QChart::SeriesAnimations);

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

    if (mTooltip != nullptr)
    {
        tooltipHideTimer.stop();

        delete mTooltip;
        mTooltip = nullptr;
    }
}

void AccountChartWidget::operationsRead(const QList<Operation>& operations)
{
    mYieldSeries.clear();
    mMonthlyYieldSeries.clear();
    mRemainedMoneySeries.clear();
    mTotalMoneySeries.clear();

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
        for (const Operation& operation : operations)
        {
            handleOperation(operation);
        }

        mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mYieldAxisY.setRange(mYieldAxisYMin, mYieldAxisYMax);
        mMonthlyYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mMonthlyYieldAxisY.setRange(mMonthlyYieldAxisYMin, mMonthlyYieldAxisYMax);
        mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin, mRemainedMoneyAxisYMax);
        mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin, mTotalMoneyAxisYMax);
    }
    else
    {
        mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
        mYieldAxisY.setRange(0, 0);
        mMonthlyYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
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
    for (const Operation& operation : operations)
    {
        handleOperation(operation);
    }

    mYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mYieldAxisY.setRange(mYieldAxisYMin, mYieldAxisYMax);
    mMonthlyYieldAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mMonthlyYieldAxisY.setRange(mMonthlyYieldAxisYMin, mMonthlyYieldAxisYMax);
    mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin, mRemainedMoneyAxisYMax);
    mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin, mTotalMoneyAxisYMax);

    scene()->invalidate();
}

void AccountChartWidget::handleOperation(const Operation& operation)
{
    const float yield         = mYieldSeries.count();         // TODO: Calculate
    const float monthlyYield  = -mMonthlyYieldSeries.count(); // TODO: Calculate
    const float remainedMoney = quotationToFloat(operation.remainedMoney);
    const float totalMoney    = quotationToFloat(operation.totalMoney);

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

    mYieldSeries.append(operation.timestamp, yield);
    mMonthlyYieldSeries.append(operation.timestamp, monthlyYield);
    mRemainedMoneySeries.append(operation.timestamp, remainedMoney);
    mTotalMoneySeries.append(operation.timestamp, totalMoney);
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
    qInfo() << path;
}

void AccountChartWidget::seriesHovered(QPointF point, bool state)
{
    if (mTooltip == nullptr)
    {
        mTooltip = new ChartTooltip(chart());
        mTooltip->setZValue(TOOLTIP_Z_VALUE);
    }

    if (state)
    {
        tooltipHideTimer.stop();

        const QPointF nearestPoint = findNearestPoint(point, qobject_cast<QLineSeries*>(sender())->points());

        const QString prefix =
            (mChartType == CHART_TYPE_YIELD || mChartType == CHART_TYPE_MONTHLY_YIELD) && nearestPoint.y() > 0 ? "+" : "";
        const QString suffix = mChartType == CHART_TYPE_YIELD || mChartType == CHART_TYPE_MONTHLY_YIELD ? "%" : "\u20BD";
        const QString xDescription =
            QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(nearestPoint.x())).toString(DATETIME_FORMAT);

        mTooltip->setText(QString("%1\n%2%3 %4").arg(xDescription, prefix, QString::number(nearestPoint.y(), 'f', 2), suffix));
        mTooltip->setAnchor(nearestPoint);
        mTooltip->updateGeometry();
        mTooltip->show();
    }
    else
    {
        tooltipHideTimer.start(TOOLTIP_HIDE_DELAY);
    }
}

void AccountChartWidget::tooltipHideTimerTicked()
{
    mTooltip->hide();
}

QPointF AccountChartWidget::findNearestPoint(const QPointF& point, const QList<QPointF>& seriesPoints)
{
    QPointF res = seriesPoints.constLast();

    // TODO: Use binary search and try to store last found point
    for (int i = 0; i < seriesPoints.size(); ++i)
    {
        const QPointF& seriesPoint = seriesPoints.at(i);
        const qreal    distance    = seriesPoint.x() - point.x();

        if (distance > 0)
        {
            if (i <= 0)
            {
                res = seriesPoint;

                break;
            }

            const qreal prevDistance = point.x() - seriesPoints.at(i - 1).x();

            if (prevDistance < distance)
            {
                res = seriesPoints.at(i - 1);
            }
            else
            {
                res = seriesPoint;
            }

            break;
        }
    }

    return res;
}
