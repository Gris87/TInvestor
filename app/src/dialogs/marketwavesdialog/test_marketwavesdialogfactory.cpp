#include "src/dialogs/marketwavesdialog/marketwavesdialogfactory.h"

#include <gtest/gtest.h>

#include "src/threads/marketwaves/imarketwavesthread_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_MarketWavesDialogFactory, Test_constructor_and_destructor)
{
    MarketWavesDialogFactory factory;
}

TEST(Test_MarketWavesDialogFactory, Test_newInstance)
{
    MarketWavesDialogFactory factory;

    StrictMock<MarketWavesThreadMock> marketWavesThread;
    Stock                             stock;

    std::shared_ptr<IMarketWavesDialog> dialog = factory.newInstance(&marketWavesThread, &stock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
