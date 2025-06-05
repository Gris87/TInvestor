#include "src/widgets/tableitems/loglevel/logleveltableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/tableitems/loglevel/logleveltableitemwidget.h"



LogLevelTableItemWidgetFactory::LogLevelTableItemWidgetFactory() :
    ILogLevelTableItemWidgetFactory()
{
    qDebug() << "Create LogLevelTableItemWidgetFactory";
}

LogLevelTableItemWidgetFactory::~LogLevelTableItemWidgetFactory()
{
    qDebug() << "Destroy LogLevelTableItemWidgetFactory";
}

ILogLevelTableItemWidget* LogLevelTableItemWidgetFactory::newInstance(QWidget* parent) const
{
    return new LogLevelTableItemWidget(parent);
}
