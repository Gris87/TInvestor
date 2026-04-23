#include "src/utils/logger/logger.h"

#include <QDebug>
#include <gtest/gtest.h>



class Test_Logger : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Logger::init();
    }

    void TearDown() override
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

    Logger::enableLogToFile();

    qDebug() << "Test";
    qInfo() << "Test";
    qWarning() << "Test";
    qCritical() << "Test";
}
