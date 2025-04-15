#pragma once



#include <QVariant>



class IProcessRunner
{
public:
    IProcessRunner()          = default;
    virtual ~IProcessRunner() = default;

    IProcessRunner(const IProcessRunner& another)            = delete;
    IProcessRunner& operator=(const IProcessRunner& another) = delete;

    virtual void setStandardOutputFile(const QString& fileName, QIODeviceBase::OpenMode mode = QIODeviceBase::Truncate) = 0;
    virtual void
    start(const QString& program, const QStringList& arguments = {}, QIODeviceBase::OpenMode mode = QIODeviceBase::ReadWrite) = 0;
    virtual bool waitForFinished(int msecs = 30000)                                                                           = 0;
};
