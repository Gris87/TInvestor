#include "src/websockets/websocketmanager.h"

#include <gtest/gtest.h>



class Test_WebSocketManager : public ::testing::Test
{
protected:
    void SetUp()
    {
        manager = new WebSocketManager();
    }

    void TearDown()
    {
        delete manager;
    }

    WebSocketManager *manager;
};



TEST_F(Test_WebSocketManager, Test_constructor_and_destructor)
{
}
