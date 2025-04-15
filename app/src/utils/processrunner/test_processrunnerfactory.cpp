#include "src/utils/processrunner/processrunnerfactory.h"

#include <gtest/gtest.h>



TEST(Test_ProcessRunnerFactory, Test_constructor_and_destructor)
{
    const ProcessRunnerFactory factory;
}

TEST(Test_ProcessRunnerFactory, Test_newInstance)
{
    const ProcessRunnerFactory factory;

    const std::shared_ptr<IProcessRunner> runner = factory.newInstance();
    ASSERT_TRUE(runner != nullptr);
}
