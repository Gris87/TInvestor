#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>
#include <QGraphicsLayout>

#include "src/grpc/utils.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";



AccountChartWidget::AccountChartWidget(QWidget* parent) :
    IAccountChartWidget(parent)
{
    qDebug() << "Create AccountChartWidget";

    initRemainedMoneyChart();
    initTotalMoneyChart();

    setChart(&mRemainedMoneyChart);
}

AccountChartWidget::~AccountChartWidget()
{
    qDebug() << "Destroy AccountChartWidget";
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

void AccountChartWidget::operationsRead(const QList<Operation>& operations)
{
    mRemainedMoneySeries.clear();
    mTotalMoneySeries.clear();

    mAxisXMin              = std::numeric_limits<qint64>::max();
    mAxisXMax              = std::numeric_limits<qint64>::min();
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

        mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin, mRemainedMoneyAxisYMax);
        mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
        mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin, mTotalMoneyAxisYMax);
    }
    else
    {
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

    mRemainedMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mRemainedMoneyAxisY.setRange(mRemainedMoneyAxisYMin, mRemainedMoneyAxisYMax);
    mTotalMoneyAxisX.setRange(QDateTime::fromMSecsSinceEpoch(mAxisXMin), QDateTime::fromMSecsSinceEpoch(mAxisXMax));
    mTotalMoneyAxisY.setRange(mTotalMoneyAxisYMin, mTotalMoneyAxisYMax);

    scene()->invalidate();
}

void AccountChartWidget::handleOperation(const Operation& operation)
{
    const float remainedMoney = quotationToFloat(operation.remainedMoney);
    const float totalMoney    = quotationToFloat(operation.totalMoney);

    mAxisXMin = qMin(mAxisXMin, operation.timestamp);
    mAxisXMax = qMax(mAxisXMax, operation.timestamp);

    mRemainedMoneyAxisYMin = qMin(mRemainedMoneyAxisYMin, remainedMoney);
    mRemainedMoneyAxisYMax = qMax(mRemainedMoneyAxisYMax, remainedMoney);
    mTotalMoneyAxisYMin    = qMin(mTotalMoneyAxisYMin, totalMoney);
    mTotalMoneyAxisYMax    = qMax(mTotalMoneyAxisYMax, totalMoney);

    mRemainedMoneySeries.append(operation.timestamp, remainedMoney);
    mTotalMoneySeries.append(operation.timestamp, totalMoney);
}
