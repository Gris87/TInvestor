#pragma once



#include "src/utils/processrunner/iprocessrunner.h"

#include <QProcess>



class ProcessRunner : public IProcessRunner
{
public:
    explicit ProcessRunner();
    ~ProcessRunner() override;

    ProcessRunner(const ProcessRunner& another)            = delete;
    ProcessRunner& operator=(const ProcessRunner& another) = delete;

    void setStandardOutputFile(const QString& fileName, QIODeviceBase::OpenMode mode = QIODeviceBase::Truncate) override;
    void start(const QString& program, const QStringList& arguments = {}, QIODeviceBase::OpenMode mode = QIODeviceBase::ReadWrite)
        override;
    bool waitForFinished(int msecs = 30000) override;

private:
    QProcess mProcess;
};
