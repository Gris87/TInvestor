#include "src/widgets/tablerecords/logstablerecord/logstablerecord.h"

#include <gtest/gtest.h>

#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidget_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LogsTableRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        logLevelTableItemWidgetFactoryMock = new StrictMock<LogLevelTableItemWidgetFactoryMock>();
        logLevelTableItemWidgetMock = new StrictMock<LogLevelTableItemWidgetMock>(nullptr); // tableWidget will take ownership
        tableWidget                 = new QTableWidget();

        tableWidget->setColumnCount(LOGS_COLUMN_COUNT);

        EXPECT_CALL(*logLevelTableItemWidgetFactoryMock, newInstance(tableWidget)).WillOnce(Return(logLevelTableItemWidgetMock));

        record = new LogsTableRecord(tableWidget, logLevelTableItemWidgetFactoryMock);
    }

    void TearDown() override
    {
        delete record;
        delete logLevelTableItemWidgetFactoryMock;
        // It will be deleted by tableWidget
        /*
        delete logLevelTableItemWidgetMock;
        */
        delete tableWidget;
    }

    LogsTableRecord*                                record;
    StrictMock<LogLevelTableItemWidgetFactoryMock>* logLevelTableItemWidgetFactoryMock;
    StrictMock<LogLevelTableItemWidgetMock>*        logLevelTableItemWidgetMock;
    QTableWidget*                                   tableWidget;
};



TEST_F(Test_LogsTableRecord, Test_constructor_and_destructor)
{
}
