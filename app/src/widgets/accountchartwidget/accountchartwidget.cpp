#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>



AccountChartWidget::AccountChartWidget(QWidget* parent) :
    IAccountChartWidget(parent)
{
    qDebug() << "Create AccountChartWidget";
}

AccountChartWidget::~AccountChartWidget()
{
    qDebug() << "Destroy AccountChartWidget";
}

void AccountChartWidget::operationsRead(const QList<Operation>& operations)
{
    qInfo() << operations.size(); // TODO: Implement
}

void AccountChartWidget::operationsAdded(const QList<Operation>& operations)
{
    qInfo() << operations.size(); // TODO: Implement
}
