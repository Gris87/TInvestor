#include "src/utils/filedialog/filedialog.h"

#include <QCoreApplication>
#include <gtest/gtest.h>



class Test_FileDialog : public ::testing::Test
{
protected:
    void SetUp() override
    {
        dialog = new FileDialog(nullptr, "", "", "");
    }

    void TearDown() override
    {
        delete dialog;
    }

    FileDialog* dialog;
};



TEST_F(Test_FileDialog, Test_constructor_and_destructor)
{
}

TEST_F(Test_FileDialog, Test_setAcceptMode)
{
    dialog->setAcceptMode(QFileDialog::AcceptSave);
}

TEST_F(Test_FileDialog, Test_setDefaultSuffix)
{
    dialog->setDefaultSuffix("xlsx");
}

TEST_F(Test_FileDialog, Test_selectFile)
{
    dialog->selectFile(qApp->applicationDirPath() + "/1.xlsx");
}

TEST_F(Test_FileDialog, Test_exec)
{
    // Nothing
}

TEST_F(Test_FileDialog, Test_selectedFiles)
{
    QStringList files = dialog->selectedFiles();

    qInfo() << files; // TODO: Remove

    ASSERT_EQ(files, QStringList());
}
