#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory_mock.h"



using ::testing::StrictMock;



class Test_PortfolioTreeRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentWidgetFactoryMock = new StrictMock<InstrumentWidgetFactoryMock>();
        userStorageMock             = new StrictMock<UserStorageMock>();
        instrumentsStorageMock      = new StrictMock<InstrumentsStorageMock>();

        treeWidget = new QTreeWidget();
        treeWidget->setColumnCount(PORTFOLIO_COLUMN_COUNT);

        categoryTreeItem = new CategoryTreeItem(treeWidget, "Hello");

        record = new PortfolioTreeRecord(
            instrumentWidgetFactoryMock, userStorageMock, instrumentsStorageMock, categoryTreeItem, "aaaaa"
        );
    }

    void TearDown() override
    {
        delete record;
        delete instrumentWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        delete treeWidget;
        // It will be deleted by treeWidget
        /*
        delete categoryTreeItem;
        */
    }

    PortfolioTreeRecord*                     record;
    StrictMock<InstrumentWidgetFactoryMock>* instrumentWidgetFactoryMock;
    StrictMock<UserStorageMock>*             userStorageMock;
    StrictMock<InstrumentsStorageMock>*      instrumentsStorageMock;
    QTreeWidget*                             treeWidget;
    CategoryTreeItem*                        categoryTreeItem;
};



TEST_F(Test_PortfolioTreeRecord, Test_constructor_and_destructor)
{
}
