#include "src/widgets/tabledelegates/loglevelitemdelegate.h"

#include <QPainter>
#include <gtest/gtest.h>

#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LogLevelItemDelegate : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logsTableModelMock = new StrictMock<LogsTableModelMock>();

        item = new LogLevelItemDelegate();
    }

    void TearDown() override
    {
        delete item;
        delete logsTableModelMock;
    }

    LogLevelItemDelegate*           item;
    StrictMock<LogsTableModelMock>* logsTableModelMock;
};



TEST_F(Test_LogLevelItemDelegate, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogLevelItemDelegate, Test_paint)
{
    const InSequence seq;

    EXPECT_CALL(*logsTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(*logsTableModelMock, columnCount(QModelIndex())).WillOnce(Return(1));

    QPainter             painter;
    QStyleOptionViewItem option;
    QModelIndex          index = logsTableModelMock->index(0, 0);

    EXPECT_CALL(*logsTableModelMock, data(index, Qt::DisplayRole)).WillOnce(Return(QVariant(0)));

    item->paint(&painter, option, index);
}

TEST_F(Test_LogLevelItemDelegate, Test_sizeHint)
{
    const InSequence seq;

    EXPECT_CALL(*logsTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(*logsTableModelMock, columnCount(QModelIndex())).WillOnce(Return(1));

    QStyleOptionViewItem option;
    QModelIndex          index = logsTableModelMock->index(0, 0);

    ASSERT_EQ(item->sizeHint(option, index), QSize(24, 24));
}
