#include "src/threads/request/requestthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/db/notifications/inotificationsdatabase_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/optimizer/ioptimizer_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_RequestThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock                = new StrictMock<ConfigMock>();
        notificationsDatabaseMock = new StrictMock<NotificationsDatabaseMock>();
        httpClientMock            = new StrictMock<HttpClientMock>();
        optimizerMock             = new StrictMock<OptimizerMock>();

        thread = new RequestThread(configMock, notificationsDatabaseMock, httpClientMock, optimizerMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete notificationsDatabaseMock;
        delete httpClientMock;
        delete optimizerMock;
    }

    RequestThread*                         thread;
    StrictMock<ConfigMock>*                configMock;
    StrictMock<NotificationsDatabaseMock>* notificationsDatabaseMock;
    StrictMock<HttpClientMock>*            httpClientMock;
    StrictMock<OptimizerMock>*             optimizerMock;
};



TEST_F(Test_RequestThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_RequestThread, Test_run)
{
}

TEST_F(Test_RequestThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
