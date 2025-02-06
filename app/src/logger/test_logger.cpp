#include "src/logger/logger.h"

#include <gtest/gtest.h>
#include <QDebug>



class Test_Logger : public ::testing::Test
{
protected:
    void SetUp()
    {
        Logger::init();
    }

    void TearDown()
    {
        Logger::deinit();
    }
};



TEST_F(Test_Logger, Test_init_deinit)
{
}

TEST_F(Test_Logger, Test_messageHandler)
{
    qDebug() << "Test";
    qInfo() << "Test";
    qWarning() << "Test";
    qCritical() << "Test";
}
