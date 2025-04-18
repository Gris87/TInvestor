#include "src/utils/autorunenabler/autorunenabler.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/fs/file/ifilefactory_mock.h"
#include "src/utils/processrunner/iprocessrunner_mock.h"
#include "src/utils/processrunner/iprocessrunnerfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_AutorunEnabler : public ::testing::Test
{
protected:
    void SetUp() override
    {
        autorunSettingsEditorMock = new StrictMock<SettingsEditorMock>();
        processRunnerFactoryMock  = new StrictMock<ProcessRunnerFactoryMock>();
        fileFactoryMock           = new StrictMock<FileFactoryMock>();

        enabler = new AutorunEnabler(autorunSettingsEditorMock, processRunnerFactoryMock, fileFactoryMock);
    }

    void TearDown() override
    {
        delete enabler;
        delete autorunSettingsEditorMock;
    }

    AutorunEnabler*                       enabler;
    StrictMock<SettingsEditorMock>*       autorunSettingsEditorMock;
    StrictMock<ProcessRunnerFactoryMock>* processRunnerFactoryMock;
    StrictMock<FileFactoryMock>*          fileFactoryMock;
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
    // const InSequence seq;

    // const QStringList crontabListArgs = QStringList() << "-l";

    // StrictMock<ProcessRunnerMock>* processRunnerMock = new StrictMock<ProcessRunnerMock>(); // Will be deleted in setEnabled

    // EXPECT_CALL(*processRunnerFactoryMock, newInstance()).WillOnce(Return(std::shared_ptr<IProcessRunner>(processRunnerMock)));
    // EXPECT_CALL(
    //     *processRunnerMock,
    //     setStandardOutputFile(QString("/tmp/TInvestor_autorun_cron"), QIODeviceBase::OpenMode(QIODeviceBase::Truncate))
    // );
    // EXPECT_CALL(
    //     *processRunnerMock, start(QString("crontab"), crontabListArgs, QIODeviceBase::OpenMode(QIODeviceBase::ReadWrite))
    // );
    // EXPECT_CALL(*processRunnerMock, waitForFinished(30000)).WillOnce(Return(false));

    //enabler->setEnabled(true);
    //enabler->setEnabled(true);
    //enabler->setEnabled(false);
}
#endif
