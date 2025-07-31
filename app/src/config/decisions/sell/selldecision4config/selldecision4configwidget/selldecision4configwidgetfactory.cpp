#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/selldecision4configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/selldecision4configwidget.h"



SellDecision4ConfigWidgetFactory::SellDecision4ConfigWidgetFactory() :
    ISellDecision4ConfigWidgetFactory()
{
    qDebug() << "Create SellDecision4ConfigWidgetFactory";
}

SellDecision4ConfigWidgetFactory::~SellDecision4ConfigWidgetFactory()
{
    qDebug() << "Destroy SellDecision4ConfigWidgetFactory";
}

ISellDecision4ConfigWidget* SellDecision4ConfigWidgetFactory::newInstance(ISellDecision4Config* config, QWidget* parent) const
{
    return new SellDecision4ConfigWidget(config, parent);
}
