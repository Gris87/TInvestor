#include "src/dialogs/marketwavesdialog/marketwavesdialog.h"
#include "ui_marketwavesdialog.h"

#include <gtest/gtest.h>

#include "src/threads/marketwaves/imarketwavesthread_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_MarketWavesDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        marketWavesThread = new StrictMock<MarketWavesThreadMock>();
        stock             = new Stock();

        dialog = new MarketWavesDialog(marketWavesThread, stock);
    }

    void TearDown()
    {
        delete dialog;
        delete marketWavesThread;
        delete stock;
    }

    MarketWavesDialog*                 dialog;
    StrictMock<MarketWavesThreadMock>* marketWavesThread;
    Stock*                             stock;
};



TEST_F(Test_MarketWavesDialog, Test_constructor_and_destructor)
{
}
