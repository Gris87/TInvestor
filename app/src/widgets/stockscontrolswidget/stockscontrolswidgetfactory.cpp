#include "src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.h"

#include <QDebug>

#include "src/widgets/stockscontrolswidget/stockscontrolswidget.h"



StocksControlsWidgetFactory::StocksControlsWidgetFactory() :
    IStocksControlsWidgetFactory()
{
    qDebug() << "Create StocksControlsWidgetFactory";
}

StocksControlsWidgetFactory::~StocksControlsWidgetFactory()
{
    qDebug() << "Destroy StocksControlsWidgetFactory";
}

IStocksControlsWidget*
StocksControlsWidgetFactory::newInstance(IStocksStorage* stocksStorage, ISettingsEditor* settingsEditor, QWidget* parent) const
{
    return new StocksControlsWidget(stocksStorage, settingsEditor, parent);
}
