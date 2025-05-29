#include "src/widgets/instrumentwidget/instrumentwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



TEST(Test_InstrumentWidgetFactory, Test_constructor_and_destructor)
{
    const InstrumentWidgetFactory factory;
}

TEST(Test_InstrumentWidgetFactory, Test_newInstance)
{
    const InstrumentWidgetFactory factory;

    StrictMock<UserStorageMock> userStorageMock;

    const IInstrumentWidget* widget = factory.newInstance(&userStorageMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
