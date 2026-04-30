#include "src/widgets/tablemodels/notificationstablemodel/notificationstablemodel.h"

#include <gtest/gtest.h>



class Test_NotificationsTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new NotificationsTableModel();
    }

    void TearDown() override
    {
        delete model;
    }

    NotificationsTableModel* model;
};



TEST_F(Test_NotificationsTableModel, Test_constructor_and_destructor)
{
}
