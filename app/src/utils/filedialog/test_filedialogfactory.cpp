#include "src/utils/filedialog/filedialogfactory.h"

#include <gtest/gtest.h>



class Test_FileDialogFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new FileDialogFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    FileDialogFactory* factory;
};



TEST_F(Test_FileDialogFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_FileDialogFactory, Test_newInstance)
{
    const std::shared_ptr<IFileDialog> dialog = factory->newInstance(nullptr, "", "", "");
    ASSERT_TRUE(dialog != nullptr);
}
