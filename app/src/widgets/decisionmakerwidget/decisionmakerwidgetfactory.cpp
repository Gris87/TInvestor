#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <QDebug>

#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"



DecisionMakerWidgetFactory::DecisionMakerWidgetFactory() :
    IDecisionMakerWidgetFactory()
{
    qDebug() << "Create DecisionMakerWidgetFactory";
}

DecisionMakerWidgetFactory::~DecisionMakerWidgetFactory()
{
    qDebug() << "Destroy DecisionMakerWidgetFactory";
}

IDecisionMakerWidget* DecisionMakerWidgetFactory::newInstance(QWidget* parent)
{
    return new DecisionMakerWidget(parent);
}
