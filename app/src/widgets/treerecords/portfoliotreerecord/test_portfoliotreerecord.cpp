#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/instrumentwidget/iinstrumentwidget_mock.h"
#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_PortfolioTreeRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        instrumentWidgetFactoryMock = new StrictMock<InstrumentWidgetFactoryMock>();
        userStorageMock             = new StrictMock<UserStorageMock>();
        instrumentsStorageMock      = new StrictMock<InstrumentsStorageMock>();

        // treeWidget will take ownership
        StrictMock<InstrumentWidgetMock>* instrumentWidgetMock = new StrictMock<InstrumentWidgetMock>();

        QMutex      mutex;
        Instruments instruments;
        Instrument  instrument;

        instrument.ticker         = "WAGA";
        instrument.name           = "Wata Giga";
        instrument.pricePrecision = 3;

        instruments["aaaaa"] = instrument;

        treeWidget = new QTreeWidget();
        treeWidget->setColumnCount(PORTFOLIO_COLUMN_COUNT);

        categoryTreeItem = new CategoryTreeItem(treeWidget, "Hello");

        EXPECT_CALL(*instrumentWidgetFactoryMock, newInstance(userStorageMock, treeWidget))
            .WillOnce(Return(instrumentWidgetMock));
        EXPECT_CALL(*instrumentsStorageMock, getMutex()).WillOnce(Return(&mutex));
        EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
        EXPECT_CALL(*instrumentWidgetMock, setInstrumentLogo(_));
        EXPECT_CALL(*instrumentWidgetMock, setTicker(QString("WAGA")));
        EXPECT_CALL(*instrumentWidgetMock, setFullText(QString("Wata Giga")));

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
