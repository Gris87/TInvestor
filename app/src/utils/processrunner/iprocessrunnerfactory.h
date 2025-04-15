#pragma once



#include "src/utils/processrunner/iprocessrunner.h"



class IProcessRunnerFactory
{
public:
    IProcessRunnerFactory()          = default;
    virtual ~IProcessRunnerFactory() = default;

    IProcessRunnerFactory(const IProcessRunnerFactory& another)            = delete;
    IProcessRunnerFactory& operator=(const IProcessRunnerFactory& another) = delete;

    [[nodiscard]]
    virtual std::shared_ptr<IProcessRunner> newInstance() const = 0;
};
