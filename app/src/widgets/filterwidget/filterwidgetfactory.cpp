#include "src/widgets/filterwidget/filterwidgetfactory.h"

#include <QDebug>

#include "src/widgets/filterwidget/filterwidget.h"



FilterWidgetFactory::FilterWidgetFactory() :
    IFilterWidgetFactory()
{
    qDebug() << "Create FilterWidgetFactory";
}

FilterWidgetFactory::~FilterWidgetFactory()
{
    qDebug() << "Destroy FilterWidgetFactory";
}

IFilterWidget* FilterWidgetFactory::newInstance(IStocksStorage* stocksStorage, QWidget* parent)
{
    return new FilterWidget(stocksStorage, parent);
}
