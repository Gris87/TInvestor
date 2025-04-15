#include "src/utils/processrunner/processrunner.h"

#include <QDebug>



ProcessRunner::ProcessRunner() :
    IProcessRunner(),
    mProcess()
{
    qDebug() << "Create ProcessRunner";
}

ProcessRunner::~ProcessRunner()
{
    qDebug() << "Destroy ProcessRunner";
}

void ProcessRunner::setStandardOutputFile(const QString& fileName, QIODeviceBase::OpenMode mode)
{
    mProcess.setStandardOutputFile(fileName, mode);
}

void ProcessRunner::start(const QString& program, const QStringList& arguments, QIODeviceBase::OpenMode mode)
{
    mProcess.start(program, arguments, mode);
}

bool ProcessRunner::waitForFinished(int msecs)
{
    return mProcess.waitForFinished(msecs);
}
