#include "src/widgets/tableitems/instrument/instrumenttableitemwidget.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
class Test_InstrumentTableItemWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        widget = new InstrumentTableItemWidget(userStorageMock);
    }

    void TearDown() override
    {
        delete widget;
        delete userStorageMock;
    }

    InstrumentTableItemWidget*   widget;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_InstrumentTableItemWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_InstrumentTableItemWidget, Test_less)
{
    InstrumentTableItemWidget widget2(userStorageMock);

    widget->setTicker("AAAAA");
    widget2.setTicker("BBBBB");

    ASSERT_LT(*widget, widget2);

    widget->setTicker("CCCCC");

    ASSERT_LT(widget2, *widget);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
