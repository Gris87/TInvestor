#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/private/qcoreapplication_p.h>
#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/trayicon/itrayicon_mock.h"
#include "src/widgets/trayicon/itrayiconfactory_mock.h"



using ::testing::_;
using ::testing::DoubleEq;
using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



#ifdef Q_OS_WINDOWS
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::DoubleClick;
#else
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::Trigger;
#endif



class Test_MainWindow : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        trayIconFactoryMock = new StrictMock<TrayIconFactoryMock>();
        settingsEditorMock  = new StrictMock<SettingsEditorMock>();
        trayIconMock        = new StrictMock<TrayIconMock>();

        EXPECT_CALL(*trayIconFactoryMock, newInstance(NotNull())).WillOnce(Return(trayIconMock));

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/geometry"),    QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/windowState"), QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        // clang-format on

        mainWindow = new MainWindow(trayIconFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        const InSequence seq;

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/geometry"),    _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/windowState"), _));
        // clang-format on

        delete mainWindow;
        delete trayIconFactoryMock;
        delete settingsEditorMock;
        delete trayIconMock;
    }

    MainWindow*                      mainWindow;
    StrictMock<TrayIconFactoryMock>* trayIconFactoryMock;
    StrictMock<SettingsEditorMock>*  settingsEditorMock;
    StrictMock<TrayIconMock>*        trayIconMock;
};



TEST_F(Test_MainWindow, Test_constructor_and_destructor)
{
}

TEST_F(Test_MainWindow, Test_closeEvent)
{
    QCloseEvent event;

    mainWindow->closeEvent(&event);

    QCoreApplicationPrivate::setEventSpontaneous(&event, true);
    mainWindow->show();

    mainWindow->closeEvent(&event);
}

TEST_F(Test_MainWindow, Test_trayIconClicked)
{
    mainWindow->trayIconClicked(DOUBLE_CLICK_REASON);

    ASSERT_EQ(mainWindow->isVisible(), true);
}

TEST_F(Test_MainWindow, Test_trayIconShowClicked)
{
    mainWindow->trayIconShowClicked();

    ASSERT_EQ(mainWindow->isVisible(), true);
}

TEST_F(Test_MainWindow, Test_trayIconExitClicked)
{
    mainWindow->trayIconExitClicked();
}
