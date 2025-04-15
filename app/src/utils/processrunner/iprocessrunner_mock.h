#pragma once



#include "src/utils/processrunner/iprocessrunner.h"

#include <gmock/gmock.h>



class ProcessRunnerMock : public IProcessRunner
{
public:
    ProcessRunnerMock() :
        IProcessRunner()
    {
    }
    ~ProcessRunnerMock() override = default;

    ProcessRunnerMock(const ProcessRunnerMock& another)            = delete;
    ProcessRunnerMock& operator=(const ProcessRunnerMock& another) = delete;

    MOCK_METHOD(void, setStandardOutputFile, (const QString& fileName, QIODeviceBase::OpenMode mode), (override));
    MOCK_METHOD(void, start, (const QString& program, const QStringList& arguments, QIODeviceBase::OpenMode mode), (override));
    MOCK_METHOD(bool, waitForFinished, (int msecs), (override));
};
