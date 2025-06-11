#include "src/widgets/logsfilterwidget/logsfilterwidgetfactory.h"

#include <QDebug>

#include "src/widgets/logsfilterwidget/logsfilterwidget.h"



LogsFilterWidgetFactory::LogsFilterWidgetFactory() :
    ILogsFilterWidgetFactory()
{
    qDebug() << "Create LogsFilterWidgetFactory";
}

LogsFilterWidgetFactory::~LogsFilterWidgetFactory()
{
    qDebug() << "Destroy LogsFilterWidgetFactory";
}

ILogsFilterWidget* LogsFilterWidgetFactory::newInstance(QWidget* parent) const
{
    return new LogsFilterWidget(parent);
}
