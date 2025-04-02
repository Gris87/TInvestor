#include "src/widgets/filterwidget/filterwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/stocks/istocksstorage_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_FilterWidgetFactory, Test_constructor_and_destructor)
{
    FilterWidgetFactory factory;
}

TEST(Test_FilterWidgetFactory, Test_newInstance)
{
    StrictMock<StocksStorageMock> stocksStorageMock;

    FilterWidgetFactory factory;

    IFilterWidget* widget = factory.newInstance(&stocksStorageMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
