#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecordfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory_mock.h"



using ::testing::StrictMock;



TEST(Test_PortfolioTreeRecordFactory, Test_constructor_and_destructor)
{
    const PortfolioTreeRecordFactory factory;
}

TEST(Test_PortfolioTreeRecordFactory, Test_newInstance)
{
    const PortfolioTreeRecordFactory factory;

    StrictMock<InstrumentWidgetFactoryMock> instrumentWidgetFactoryMock;
    StrictMock<UserStorageMock>             userStorageMock;
    StrictMock<InstrumentsStorageMock>      instrumentsStorageMock;

    QTreeWidget treeWidget;
    treeWidget.setColumnCount(PORTFOLIO_COLUMN_COUNT);

    CategoryTreeItem categoryTreeItem(&treeWidget, "Hello");

    const IPortfolioTreeRecord* record = factory.newInstance(
        &instrumentWidgetFactoryMock, &userStorageMock, &instrumentsStorageMock, &categoryTreeItem, "aaaaa", nullptr
    );
    ASSERT_TRUE(record != nullptr);

    delete record;
}
