#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>
#include <QGraphicsLayout>

#include "src/grpc/utils.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";



AccountChartWidget::AccountChartWidget(QWidget* parent) :
    IAccountChartWidget(parent)
{
    qDebug() << "Create AccountChartWidget";

    initYieldChart();
    initMonthlyYieldChart();
    initRemainedMoneyChart();
    initTotalMoneyChart();

    setChart(&mYieldChart);
}

AccountChartWidget::~AccountChartWidget()
{
    qDebug() << "Destroy AccountChartWidget";
}

void AccountChartWidget::initYieldChart()
{
    mYieldChart.layout()->setContentsMargins(0, 0, 0, 0);
    mYieldChart.setBackgroundRoundness(0);

    mYieldChart.setTitle(tr("Yield"));
    mYieldChart.addSeries(&mYieldSeries);
    mYieldChart.legend()->hide();

    mYieldAxisX.setFormat(DATETIME_FORMAT);
    mYieldAxisX.setTitleText(tr("Time"));
    mYieldChart.addAxis(&mYieldAxisX, Qt::AlignBottom);
    mYieldSeries.attachAxis(&mYieldAxisX);

    mYieldAxisY.setLabelFormat("%g");
    mYieldAxisY.setTitleText("%");
    mYieldChart.addAxis(&mYieldAxisY, Qt::AlignLeft);
    mYieldSeries.attachAxis(&mYieldAxisY);
}

void AccountChartWidget::initMonthlyYieldChart()
{
    mMonthlyYieldChart.layout()->setContentsMargins(0, 0, 0, 0);
    mMonthlyYieldChart.setBackgroundRoundness(0);

    mMonthlyYieldChart.setTitle(tr("Yield per month"));
    mMonthlyYieldChart.addSeries(&mMonthlyYieldSeries);
    mMonthlyYieldChart.legend()->hide();

    mMonthlyYieldAxisX.setFormat(DATETIME_FORMAT);
    mMonthlyYieldAxisX.setTitleText(tr("Time"));
    mMonthlyYieldChart.addAxis(&mMonthlyYieldAxisX, Qt::AlignBottom);
    mMonthlyYieldSeries.attachAxis(&mMonthlyYieldAxisX);

    mMonthlyYieldAxisY.setLabelFormat("%g");
    mMonthlyYieldAxisY.setTitleText("%");
    mMonthlyYieldChart.addAxis(&mMonthlyYieldAxisY, Qt::AlignLeft);
    mMonthlyYieldSeries.attachAxis(&mMonthlyYieldAxisY);
}

void AccountChartWidget::initRemainedMoneyChart()
{
    mRemainedMoneyChart.layout()->setContentsMargins(0, 0, 0, 0);
    mRemainedMoneyChart.setBackgroundRoundness(0);

    mRemainedMoneyChart.setTitle(tr("Remained money on account"));
    mRemainedMoneyChart.addSeries(&mRemainedMoneySeries);
    mRemainedMoneyChart.legend()->hide();

    mRemainedMoneyAxisX.setFormat(DATETIME_FORMAT);
    mRemainedMoneyAxisX.setTitleText(tr("Time"));
    mRemainedMoneyChart.addAxis(&mRemainedMoneyAxisX, Qt::AlignBottom);
    mRemainedMoneySeries.attachAxis(&mRemainedMoneyAxisX);

    mRemainedMoneyAxisY.setLabelFormat("%g");
    mRemainedMoneyAxisY.setTitleText(tr("Money") + ", \u20BD");
    mRemainedMoneyChart.addAxis(&mRemainedMoneyAxisY, Qt::AlignLeft);
    mRemainedMoneySeries.attachAxis(&mRemainedMoneyAxisY);
}

void AccountChartWidget::initTotalMoneyChart()
{
    mTotalMoneyChart.layout()->setContentsMargins(0, 0, 0, 0);
    mTotalMoneyChart.setBackgroundRoundness(0);

    mTotalMoneyChart.setTitle(tr("Total money on account"));
    mTotalMoneyChart.addSeries(&mTotalMoneySeries);
    mTotalMoneyChart.legend()->hide();

    mTotalMoneyAxisX.setFormat(DATETIME_FORMAT);
    mTotalMoneyAxisX.setTitleText(tr("Time"));
    mTotalMoneyChart.addAxis(&mTotalMoneyAxisX, Qt::AlignBottom);
    mTotalMoneySeries.attachAxis(&mTotalMoneyAxisX);

    mTotalMoneyAxisY.setLabelFormat("%g");
    mTotalMoneyAxisY.setTitleText(tr("Money") + ", \u20BD");
    mTotalMoneyChart.addAxis(&mTotalMoneyAxisY, Qt::AlignLeft);
    mTotalMoneySeries.attachAxis(&mTotalMoneyAxisY);
}

void AccountChartWidget::switchToYieldChart()
{
    setChart(&mYieldChart);
}

void AccountChartWidget::switchToMonthlyYieldChart()
{
    setChart(&mMonthlyYieldChart);
}

void AccountChartWidget::switchToRemainedMoneyChart()
{
    setChart(&mRemainedMoneyChart);
}

void AccountChartWidget::switchToTotalMoneyChart()
{
    setChart(&mTotalMoneyChart);
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
    const float yield         = mYieldSeries.count(); // TODO: Calculate
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
