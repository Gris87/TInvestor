#include "src/dialogs/marketwavesdialog/marketwavesdialog.h"
#include "ui_marketwavesdialog.h"

#include <gtest/gtest.h>



class Test_MarketWavesDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        dialog = new MarketWavesDialog();
    }

    void TearDown()
    {
        delete dialog;
    }

    MarketWavesDialog* dialog;
};



TEST_F(Test_MarketWavesDialog, Test_constructor_and_destructor)
{
}
