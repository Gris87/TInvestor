#include "src/widgets/tableitems/loglevel/logleveltableitemwidgetfactory.h"

#include <gtest/gtest.h>



TEST(Test_LogLevelTableItemWidgetFactory, Test_constructor_and_destructor)
{
    const LogLevelTableItemWidgetFactory factory;
}

TEST(Test_LogLevelTableItemWidgetFactory, Test_newInstance)
{
    const LogLevelTableItemWidgetFactory factory;

    const ILogLevelTableItemWidget* widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
