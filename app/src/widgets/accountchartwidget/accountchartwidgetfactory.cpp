#include "src/widgets/accountchartwidget/accountchartwidgetfactory.h"

#include <QDebug>

#include "src/widgets/accountchartwidget/accountchartwidget.h"



AccountChartWidgetFactory::AccountChartWidgetFactory() :
    IAccountChartWidgetFactory()
{
    qDebug() << "Create AccountChartWidgetFactory";
}

AccountChartWidgetFactory::~AccountChartWidgetFactory()
{
    qDebug() << "Destroy AccountChartWidgetFactory";
}

IAccountChartWidget* AccountChartWidgetFactory::newInstance(QWidget* parent) const
{
    return new AccountChartWidget(parent);
}
