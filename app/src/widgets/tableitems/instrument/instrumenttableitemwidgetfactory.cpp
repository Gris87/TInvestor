#include "src/widgets/tableitems/instrument/instrumenttableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/tableitems/instrument/instrumenttableitemwidget.h"



InstrumentTableItemWidgetFactory::InstrumentTableItemWidgetFactory() :
    IInstrumentTableItemWidgetFactory()
{
    qDebug() << "Create InstrumentTableItemWidgetFactory";
}

InstrumentTableItemWidgetFactory::~InstrumentTableItemWidgetFactory()
{
    qDebug() << "Destroy InstrumentTableItemWidgetFactory";
}

IInstrumentTableItemWidget* InstrumentTableItemWidgetFactory::newInstance(IUserStorage* userStorage, QWidget* parent) const
{
    return new InstrumentTableItemWidget(userStorage, parent);
}
