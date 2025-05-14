#include "src/widgets/tableitems/instrument/instrumenttableitemwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



TEST(Test_InstrumentTableItemWidgetFactory, Test_constructor_and_destructor)
{
    const InstrumentTableItemWidgetFactory factory;
}

TEST(Test_InstrumentTableItemWidgetFactory, Test_newInstance)
{
    const InstrumentTableItemWidgetFactory factory;

    StrictMock<UserStorageMock> userStorageMock;

    const IInstrumentTableItemWidget* widget = factory.newInstance(&userStorageMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
