#pragma once



#include "src/utils/processrunner/iprocessrunnerfactory.h"



class ProcessRunnerFactory : public IProcessRunnerFactory
{
public:
    ProcessRunnerFactory();
    ~ProcessRunnerFactory() override;

    ProcessRunnerFactory(const ProcessRunnerFactory& another)            = delete;
    ProcessRunnerFactory& operator=(const ProcessRunnerFactory& another) = delete;

    [[nodiscard]]
    std::shared_ptr<IProcessRunner> newInstance() const override;
};
