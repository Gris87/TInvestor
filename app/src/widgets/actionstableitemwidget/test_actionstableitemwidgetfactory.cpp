#include "src/widgets/actionstableitemwidget/actionstableitemwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel_mock.h"



using ::testing::StrictMock;



class Test_ActionsTableItemWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new ActionsTableItemWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    ActionsTableItemWidgetFactory* factory;
};



TEST_F(Test_ActionsTableItemWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_ActionsTableItemWidgetFactory, Test_newInstance)
{
    StrictMock<OrderWavesDialogFactoryMock> orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    StrictMock<HttpClientMock>              httpClientMock;
    StrictMock<StocksTableModelMock>        stocksTableModelMock;

    const IActionsTableItemWidget* widget = factory->newInstance(
        &orderWavesDialogFactoryMock,
        &orderWavesWidgetFactoryMock,
        &orderBookThreadMock,
        &httpClientMock,
        &stocksTableModelMock,
        0,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
