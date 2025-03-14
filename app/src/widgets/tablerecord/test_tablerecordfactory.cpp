#include "src/widgets/tablerecord/tablerecordfactory.h"

#include <gtest/gtest.h>

#include "src/utils/http/ihttpclient_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_TableRecordFactory, Test_constructor_and_destructor)
{
    TableRecordFactory factory;
}

TEST(Test_TableRecordFactory, Test_newInstance)
{
    StrictMock<HttpClientMock> httpClientMock;

    TableRecordFactory factory;

    QTableWidget tableWidget;
    Stock        stock;

    ITableRecord* record = factory.newInstance(&tableWidget, &httpClientMock, &stock, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
