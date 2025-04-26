#include "src/utils/autorunenabler/autorunenabler.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::_; // NOLINT(misc-unused-using-decls)
using ::testing::InSequence;
using ::testing::Return; // NOLINT(misc-unused-using-decls)
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AutorunEnabler : public ::testing::Test
{
protected:
    void SetUp() override
    {
        autorunSettingsEditorMock = new StrictMock<SettingsEditorMock>();
        dirFactoryMock            = new StrictMock<DirFactoryMock>();
        fileFactoryMock           = new StrictMock<FileFactoryMock>();

        enabler = new AutorunEnabler(autorunSettingsEditorMock, dirFactoryMock, fileFactoryMock);
    }

    void TearDown() override
    {
        delete enabler;
        delete autorunSettingsEditorMock;
        delete dirFactoryMock;
        delete fileFactoryMock;
    }

    AutorunEnabler*                 enabler;
    StrictMock<SettingsEditorMock>* autorunSettingsEditorMock;
    StrictMock<DirFactoryMock>*     dirFactoryMock;
    StrictMock<FileFactoryMock>*    fileFactoryMock;
};



TEST_F(Test_AutorunEnabler, Test_constructor_and_destructor)
{
}

#ifdef Q_OS_WINDOWS
TEST_F(Test_AutorunEnabler, Test_setEnabled)
{
    const InSequence seq;

    const QString appPath = QDir::toNativeSeparators(qApp->applicationFilePath());

    EXPECT_CALL(
        *autorunSettingsEditorMock,
        setValue(QString("CurrentVersion/Run/TInvestor"), QVariant(QString("\"%1\" --autorun").arg(appPath)))
    );
    enabler->setEnabled(true);

    EXPECT_CALL(
        *autorunSettingsEditorMock,
        setValue(QString("CurrentVersion/Run/TInvestor"), QVariant(QString("\"%1\" --autorun").arg(appPath)))
    );
    enabler->setEnabled(true);

    EXPECT_CALL(*autorunSettingsEditorMock, remove(QString("CurrentVersion/Run/TInvestor")));
    enabler->setEnabled(false);
}
#else
TEST_F(Test_AutorunEnabler, Test_setEnabled)
{
    const InSequence seq;

    StrictMock<DirMock>*  dirMock   = new StrictMock<DirMock>();  // Will be deleted in enable function
    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in enable function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in disable function

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
    EXPECT_CALL(*dirMock, mkpath(QDir::homePath() + "/.config/autostart")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(QDir::homePath() + "/.config/autostart/TInvestor.desktop"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, write(_)).WillOnce(Return(1));
    EXPECT_CALL(*fileMock1, close());
    enabler->setEnabled(true);

    EXPECT_CALL(*fileFactoryMock, newInstance(QDir::homePath() + "/.config/autostart/TInvestor.desktop"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, exists()).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, remove()).WillOnce(Return(true));
    enabler->setEnabled(false);
}
#endif
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
