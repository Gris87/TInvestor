#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <gtest/gtest.h>



class Test_MainWindow : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mainWindow = new MainWindow();
    }

    void TearDown() override
    {
        delete mainWindow;
    }

    MainWindow* mainWindow;
};



TEST_F(Test_MainWindow, Test_constructor_and_destructor)
{
}
