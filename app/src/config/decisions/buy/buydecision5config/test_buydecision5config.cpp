#include "src/config/decisions/buy/buydecision5config/buydecision5config.h"

#include <gtest/gtest.h>

#include "src/utils/exception/exception.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-function-cognitive-complexity, readability-magic-numbers)
class Test_BuyDecision5Config : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config = new BuyDecision5Config();
    }

    void TearDown() override
    {
        delete config;
    }

    BuyDecision5Config* config;
};



TEST_F(Test_BuyDecision5Config, Test_constructor_and_destructor)
{
}
// NOLINTEND(readability-function-cognitive-complexity, readability-magic-numbers)
