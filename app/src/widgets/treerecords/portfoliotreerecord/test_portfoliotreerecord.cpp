#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"



using ::testing::StrictMock;



class Test_PortfolioTreeRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();

        treeWidget = new QTreeWidget();
        treeWidget->setColumnCount(PORTFOLIO_COLUMN_COUNT);

        categoryTreeItem = new CategoryTreeItem(treeWidget, "Hello");

        record = new PortfolioTreeRecord(instrumentsStorageMock, categoryTreeItem, "aaaaa");
    }

    void TearDown() override
    {
        delete record;
        delete instrumentsStorageMock;
        delete treeWidget;
        // It will be deleted by treeWidget
        /*
        delete categoryTreeItem;
        */
    }

    PortfolioTreeRecord*                record;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    QTreeWidget*                        treeWidget;
    CategoryTreeItem*                   categoryTreeItem;
};



TEST_F(Test_PortfolioTreeRecord, Test_constructor_and_destructor)
{
}
