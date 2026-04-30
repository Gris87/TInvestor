#include "src/widgets/tablemodels/notificationstablemodel/notificationstablemodelfactory.h"

#include <gtest/gtest.h>



class Test_NotificationsTableModelFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new NotificationsTableModelFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    NotificationsTableModelFactory* factory;
};



TEST_F(Test_NotificationsTableModelFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationsTableModelFactory, Test_newInstance)
{
    const INotificationsTableModel* model = factory->newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
