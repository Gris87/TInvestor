#include "src/threads/cleanupthread.h"

#include <gtest/gtest.h>

#include "src/db/stocks/istocksdatabase_mock.h"



TEST(Test_CleanupThread, Test_constructor_and_destructor)
{
    StocksDatabaseMock stocksDatabaseMock;

    CleanupThread thread(&stocksDatabaseMock);
}

TEST(Test_CleanupThread, Test_process)
{
    StocksDatabaseMock stocksDatabaseMock;

    CleanupThread thread(&stocksDatabaseMock);

    thread.process();
}
