#include "src/widgets/portfoliotablewidget/portfoliotablewidgetfactory.h"

#include <QDebug>

#include "src/widgets/portfoliotablewidget/portfoliotablewidget.h"



PortfolioTableWidgetFactory::PortfolioTableWidgetFactory() :
    IPortfolioTableWidgetFactory()
{
    qDebug() << "Create PortfolioTableWidgetFactory";
}

PortfolioTableWidgetFactory::~PortfolioTableWidgetFactory()
{
    qDebug() << "Destroy PortfolioTableWidgetFactory";
}

IPortfolioTableWidget* PortfolioTableWidgetFactory::newInstance(ISettingsEditor* settingsEditor, QWidget* parent)
{
    return new PortfolioTableWidget(settingsEditor, parent);
}
