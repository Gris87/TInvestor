#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>

#include "src/grpc/utils.h"



AccountChartWidget::AccountChartWidget(QWidget* parent) :
    IAccountChartWidget(parent)
{
    qDebug() << "Create AccountChartWidget";

    mTotalMoneyChart.addSeries(&mTotalMoneySeries);
    mTotalMoneyChart.legend()->hide();
    mTotalMoneyChart.setTitle(tr("Total money on account"));

    setChart(&mTotalMoneyChart);
}

AccountChartWidget::~AccountChartWidget()
{
    qDebug() << "Destroy AccountChartWidget";
}

void AccountChartWidget::operationsRead(const QList<Operation>& operations)
{
    mTotalMoneySeries.clear();

    for (const Operation& operation : operations)
    {
        handleOperation(operation);
    }
}

void AccountChartWidget::operationsAdded(const QList<Operation>& operations)
{
    for (const Operation& operation : operations)
    {
        handleOperation(operation);
    }
}

void AccountChartWidget::handleOperation(const Operation& operation)
{
    mTotalMoneySeries.append(QPointF(mTotalMoneySeries.count(), quotationToFloat(operation.totalMoney)));
}
