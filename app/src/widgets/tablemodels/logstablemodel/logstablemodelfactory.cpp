#include "src/widgets/tablemodels/logstablemodel/logstablemodelfactory.h"

#include <QDebug>

#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"



LogsTableModelFactory::LogsTableModelFactory() :
    ILogsTableModelFactory()
{
    qDebug() << "Create LogsTableModelFactory";
}

LogsTableModelFactory::~LogsTableModelFactory()
{
    qDebug() << "Destroy LogsTableModelFactory";
}

ILogsTableModel* LogsTableModelFactory::newInstance(QObject* parent) const
{
    return new LogsTableModel(parent);
}
