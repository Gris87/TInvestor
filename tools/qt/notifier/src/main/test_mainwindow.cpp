#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/private/qcoreapplication_p.h>
#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialog_mock.h"
#include "src/dialogs/settingsdialog/isettingsdialogfactory_mock.h"
#include "src/utils/autorunenabler/iautorunenabler_mock.h"
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

        configMock                  = new StrictMock<ConfigMock>();
        configForSettingsDialogMock = new StrictMock<ConfigMock>();
        settingsDialogFactoryMock   = new StrictMock<SettingsDialogFactoryMock>();
        trayIconFactoryMock         = new StrictMock<TrayIconFactoryMock>();
        settingsEditorMock          = new StrictMock<SettingsEditorMock>();
        autorunEnablerMock          = new StrictMock<AutorunEnablerMock>();
        trayIconMock                = new StrictMock<TrayIconMock>();

        EXPECT_CALL(*trayIconFactoryMock, newInstance(NotNull())).WillOnce(Return(trayIconMock));

        EXPECT_CALL(*configMock, makeDefault());
        EXPECT_CALL(*configMock, load(settingsEditorMock));
        EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
        EXPECT_CALL(*autorunEnablerMock, setEnabled(true));

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/geometry"),    QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/windowState"), QVariant(QByteArray()))).WillOnce(Return(QVariant(QByteArray())));
        // clang-format on

        mainWindow = new MainWindow(
            configMock,
            configForSettingsDialogMock,
            settingsDialogFactoryMock,
            trayIconFactoryMock,
            settingsEditorMock,
            autorunEnablerMock
        );
    }

    void TearDown() override
    {
        const InSequence seq;

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/geometry"),    _));
        EXPECT_CALL(*settingsEditorMock, setValue(QString("MainWindow/windowState"), _));
        // clang-format on

        delete mainWindow;
        delete configMock;
        delete configForSettingsDialogMock;
        delete settingsDialogFactoryMock;
        delete trayIconFactoryMock;
        delete settingsEditorMock;
        delete autorunEnablerMock;
        delete trayIconMock;
    }

    MainWindow*                            mainWindow;
    StrictMock<ConfigMock>*                configMock;
    StrictMock<ConfigMock>*                configForSettingsDialogMock;
    StrictMock<SettingsDialogFactoryMock>* settingsDialogFactoryMock;
    StrictMock<TrayIconFactoryMock>*       trayIconFactoryMock;
    StrictMock<SettingsEditorMock>*        settingsEditorMock;
    StrictMock<AutorunEnablerMock>*        autorunEnablerMock;
    StrictMock<TrayIconMock>*              trayIconMock;
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

TEST_F(Test_MainWindow, Test_on_actionSettings_triggered)
{
    const InSequence seq;

    // Will be deleted in on_actionSettings_triggered
    StrictMock<SettingsDialogMock>* settingsDialogMock = new StrictMock<SettingsDialogMock>();

    EXPECT_CALL(*configForSettingsDialogMock, assign(configMock));
    EXPECT_CALL(*settingsDialogFactoryMock, newInstance(configForSettingsDialogMock, mainWindow))
        .WillOnce(Return(std::shared_ptr<ISettingsDialog>(settingsDialogMock)));
    EXPECT_CALL(*settingsDialogMock, updateUiFromConfig());

    EXPECT_CALL(*settingsDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*configMock, assign(configForSettingsDialogMock));
    EXPECT_CALL(*configMock, save(settingsEditorMock));

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(false));
    EXPECT_CALL(*autorunEnablerMock, setEnabled(false));

    mainWindow->ui->actionSettings->trigger();
}
