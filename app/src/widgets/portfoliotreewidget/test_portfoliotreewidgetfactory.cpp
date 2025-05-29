#include "src/widgets/portfoliotreewidget/portfoliotreewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory_mock.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory_mock.h"



using ::testing::StrictMock;



TEST(Test_PortfolioTreeWidgetFactory, Test_constructor_and_destructor)
{
    const PortfolioTreeWidgetFactory factory;
}

TEST(Test_PortfolioTreeWidgetFactory, Test_newInstance)
{
    const PortfolioTreeWidgetFactory factory;

    StrictMock<PortfolioTreeRecordFactoryMock> portfolioTreeRecordFactoryMock;
    StrictMock<InstrumentWidgetFactoryMock>    instrumentWidgetFactoryMock;
    StrictMock<UserStorageMock>                userStorageMock;
    StrictMock<InstrumentsStorageMock>         instrumentsStorageMock;
    StrictMock<SettingsEditorMock>             settingsEditorMock;

    const IPortfolioTreeWidget* widget = factory.newInstance(
        &portfolioTreeRecordFactoryMock,
        &instrumentWidgetFactoryMock,
        &userStorageMock,
        &instrumentsStorageMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
