#include "src/utils/processrunner/processrunnerfactory.h"

#include <QDebug>

#include "src/utils/processrunner/processrunner.h"



ProcessRunnerFactory::ProcessRunnerFactory() :
    IProcessRunnerFactory()
{
    qDebug() << "Create ProcessRunnerFactory";
}

ProcessRunnerFactory::~ProcessRunnerFactory()
{
    qDebug() << "Destroy ProcessRunnerFactory";
}

std::shared_ptr<IProcessRunner> ProcessRunnerFactory::newInstance() const
{
    return std::shared_ptr<IProcessRunner>(new ProcessRunner());
}
