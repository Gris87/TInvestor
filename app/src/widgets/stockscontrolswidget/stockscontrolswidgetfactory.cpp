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

IStocksControlsWidget* StocksControlsWidgetFactory::newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const
{
    return new StocksControlsWidget(settingsEditor, parent);
}
