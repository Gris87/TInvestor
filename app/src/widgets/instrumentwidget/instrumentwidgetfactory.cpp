#include "src/widgets/instrumentwidget/instrumentwidgetfactory.h"

#include <QDebug>

#include "src/widgets/instrumentwidget/instrumentwidget.h"



InstrumentWidgetFactory::InstrumentWidgetFactory() :
    IInstrumentWidgetFactory()
{
    qDebug() << "Create InstrumentWidgetFactory";
}

InstrumentWidgetFactory::~InstrumentWidgetFactory()
{
    qDebug() << "Destroy InstrumentWidgetFactory";
}

IInstrumentWidget* InstrumentWidgetFactory::newInstance(IUserStorage* userStorage, QWidget* parent) const
{
    return new InstrumentWidget(userStorage, parent);
}
