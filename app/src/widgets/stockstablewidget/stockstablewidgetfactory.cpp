#include "src/widgets/stockstablewidget/stockstablewidgetfactory.h"

#include <QDebug>

#include "src/widgets/stockstablewidget/stockstablewidget.h"



StocksTableWidgetFactory::StocksTableWidgetFactory() :
    IStocksTableWidgetFactory()
{
    qDebug() << "Create StocksTableWidgetFactory";
}

StocksTableWidgetFactory::~StocksTableWidgetFactory()
{
    qDebug() << "Destroy StocksTableWidgetFactory";
}

IStocksTableWidget* StocksTableWidgetFactory::newInstance(ISettingsEditor* settingsEditor, QWidget* parent)
{
    return new StocksTableWidget(settingsEditor, parent);
}
