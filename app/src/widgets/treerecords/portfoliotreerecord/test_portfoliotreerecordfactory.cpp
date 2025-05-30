#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecordfactory.h"

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



TEST(Test_PortfolioTreeRecordFactory, Test_constructor_and_destructor)
{
    const PortfolioTreeRecordFactory factory;
}

TEST(Test_PortfolioTreeRecordFactory, Test_newInstance)
{
    const InSequence seq;

    const PortfolioTreeRecordFactory factory;

    StrictMock<InstrumentWidgetFactoryMock> instrumentWidgetFactoryMock;
    StrictMock<UserStorageMock>             userStorageMock;
    StrictMock<InstrumentsStorageMock>      instrumentsStorageMock;

    // treeWidget will take ownership
    StrictMock<InstrumentWidgetMock>* instrumentWidgetMock = new StrictMock<InstrumentWidgetMock>();

    QMutex      mutex;
    Instruments instruments;
    Instrument  instrument;

    instrument.ticker         = "WAGA";
    instrument.name           = "Wata Giga";
    instrument.pricePrecision = 3;

    instruments["aaaaa"] = instrument;

    QTreeWidget treeWidget;
    treeWidget.setColumnCount(PORTFOLIO_COLUMN_COUNT);

    CategoryTreeItem categoryTreeItem(&treeWidget, "Hello");

    EXPECT_CALL(instrumentWidgetFactoryMock, newInstance(&userStorageMock, &treeWidget)).WillOnce(Return(instrumentWidgetMock));
    EXPECT_CALL(instrumentsStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentWidgetMock, setInstrumentLogo(_));
    EXPECT_CALL(*instrumentWidgetMock, setTicker(QString("WAGA")));
    EXPECT_CALL(*instrumentWidgetMock, setFullText(QString("Wata Giga")));

    const IPortfolioTreeRecord* record = factory.newInstance(
        &instrumentWidgetFactoryMock, &userStorageMock, &instrumentsStorageMock, &categoryTreeItem, "aaaaa", nullptr
    );
    ASSERT_TRUE(record != nullptr);

    delete record;
}
