#include "src/utils/filedialog/filedialogfactory.h"

#include <gtest/gtest.h>



TEST(Test_FileDialogFactory, Test_constructor_and_destructor)
{
    const FileDialogFactory factory;
}

TEST(Test_FileDialogFactory, Test_newInstance)
{
    const FileDialogFactory factory;

    const std::shared_ptr<IFileDialog> dialog = factory.newInstance(nullptr, "", "", "");
    ASSERT_TRUE(dialog != nullptr);
}
