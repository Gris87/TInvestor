#include "src/widgets/tabledelegates/instrumentitemdelegate.h"

#include <QPainter>
#include <QTreeView>
#include <gtest/gtest.h>

#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel_mock.h"
#include "src/widgets/tablemodels/modelroles.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_InstrumentItemDelegate : public ::testing::Test
{
protected:
    void SetUp() override
    {
        treeView           = new QTreeView();
        logsTableModelMock = new StrictMock<LogsTableModelMock>();

        item = new InstrumentItemDelegate(treeView);
    }

    void TearDown() override
    {
        delete item;
        delete treeView;
        delete logsTableModelMock;
    }

    InstrumentItemDelegate*         item;
    QTreeView*                      treeView;
    StrictMock<LogsTableModelMock>* logsTableModelMock;
};



TEST_F(Test_InstrumentItemDelegate, Test_constructor_and_destructor)
{
}

TEST_F(Test_InstrumentItemDelegate, Test_paint)
{
    const InSequence seq;

    Logo logo;

    EXPECT_CALL(*logsTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(*logsTableModelMock, columnCount(QModelIndex())).WillOnce(Return(1));

    QPainter             painter;
    QStyleOptionViewItem option;
    QModelIndex          index = logsTableModelMock->index(0, 0);

    option.state |= QStyle::State_MouseOver;

    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOGO))
        .WillOnce(Return(QVariant(reinterpret_cast<qint64>(&logo))));
    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOCKED)).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_NAME)).WillOnce(Return(QVariant("SPB Market")));

    item->paint(&painter, option, index);

    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOGO))
        .WillOnce(Return(QVariant(reinterpret_cast<qint64>(&logo))));
    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOCKED)).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_NAME)).WillOnce(Return(QVariant("")));

    item->paint(&painter, option, index);

    option.state &= ~QStyle::State_MouseOver;

    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOGO))
        .WillOnce(Return(QVariant(reinterpret_cast<qint64>(&logo))));
    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOCKED)).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*logsTableModelMock, data(index, Qt::DisplayRole)).WillOnce(Return(QVariant("SPBE")));

    item->paint(&painter, option, index);
}

TEST_F(Test_InstrumentItemDelegate, Test_sizeHint)
{
    const InSequence seq;

    EXPECT_CALL(*logsTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(*logsTableModelMock, columnCount(QModelIndex())).WillOnce(Return(1));

    QStyleOptionViewItem option;
    QModelIndex          index = logsTableModelMock->index(0, 0);

    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOCKED)).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(*logsTableModelMock, data(index, Qt::DisplayRole)).WillOnce(Return("SPBE"));

    QSize size = item->sizeHint(option, index);

#ifdef Q_OS_WINDOWS
    ASSERT_EQ(size, QSize(57, 24));
#else
    ASSERT_TRUE(size.width() >= 63 && size.width() <= 67);
    ASSERT_EQ(size.height(), 24);
#endif

    EXPECT_CALL(*logsTableModelMock, data(index, ROLE_INSTRUMENT_LOCKED)).WillOnce(Return(QVariant(true)));
    EXPECT_CALL(*logsTableModelMock, data(index, Qt::DisplayRole)).WillOnce(Return("SPBE"));

    size = item->sizeHint(option, index);

#ifdef Q_OS_WINDOWS
    ASSERT_EQ(size, QSize(77, 24));
#else
    ASSERT_TRUE(size.width() >= 83 && size.width() <= 87);
    ASSERT_EQ(size.height(), 24);
#endif
}
