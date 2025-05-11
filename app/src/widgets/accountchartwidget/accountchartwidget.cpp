#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <QDebug>



AccountChartWidget::AccountChartWidget(QWidget* parent) :
    IAccountChartWidget(parent),
    mScene()
{
    qDebug() << "Create AccountChartWidget";

    setScene(&mScene);

    setDragMode(DragMode::ScrollHandDrag);
}

AccountChartWidget::~AccountChartWidget()
{
    qDebug() << "Destroy AccountChartWidget";
}

void AccountChartWidget::operationsRead(const QList<Operation>& operations)
{
    qInfo() << operations.size();
}

void AccountChartWidget::operationsAdded(const QList<Operation>& operations)
{
    qInfo() << operations.size();
}
