#include "src/dialogs/marketwavesdialog/marketwavesdialogfactory.h"

#include <gtest/gtest.h>



TEST(Test_MarketWavesDialogFactory, Test_constructor_and_destructor)
{
    MarketWavesDialogFactory factory;
}

TEST(Test_MarketWavesDialogFactory, Test_newInstance)
{
    MarketWavesDialogFactory factory;

    std::shared_ptr<IMarketWavesDialog> dialog = factory.newInstance(nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
