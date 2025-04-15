#pragma once



#include "src/utils/processrunner/iprocessrunnerfactory.h"

#include <gmock/gmock.h>



class ProcessRunnerFactoryMock : public IProcessRunnerFactory
{
public:
    ProcessRunnerFactoryMock() :
        IProcessRunnerFactory()
    {
    }
    ~ProcessRunnerFactoryMock() override = default;

    ProcessRunnerFactoryMock(const ProcessRunnerFactoryMock& another)            = delete;
    ProcessRunnerFactoryMock& operator=(const ProcessRunnerFactoryMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<IProcessRunner>, newInstance, (), (const, override));
};
