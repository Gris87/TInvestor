#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"
#include "ui_startautopilotdialog.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessagebox_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
class Test_StartAutoPilotDialog : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        userStorageMock     = new StrictMock<UserStorageMock>();
        messageBoxUtilsMock = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock  = new StrictMock<SettingsEditorMock>();

        QMutex mutex;

        QList<Account> accounts;

        Account account1;
        Account account2;
        Account account3;

        account1.setId("aaaa");
        account2.setId("bbbb");
        account3.setId("cccc");

        account1.name = "Babushka";
        account2.name = "Matreshka";
        account3.name = "Balalayka";

        accounts << account1 << account2 << account3;

        EXPECT_CALL(*userStorageMock, getMutex()).WillOnce(Return(&mutex));
        EXPECT_CALL(*userStorageMock, getAccounts()).WillOnce(ReturnRef(accounts));

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("StartAutoPilotDialog/account"),        QVariant(""))).WillOnce(Return(QVariant("")));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartAutoPilotDialog/follow"),         QVariant(false))).WillOnce(Return(QVariant(false)));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartAutoPilotDialog/anotherAccount"), QVariant(""))).WillOnce(Return(QVariant("")));
        // clang-format on

        dialog = new StartAutoPilotDialog(userStorageMock, messageBoxUtilsMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete dialog;
        delete userStorageMock;
        delete messageBoxUtilsMock;
        delete settingsEditorMock;
    }

    StartAutoPilotDialog*            dialog;
    StrictMock<UserStorageMock>*     userStorageMock;
    StrictMock<MessageBoxUtilsMock>* messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>*  settingsEditorMock;
};



TEST_F(Test_StartAutoPilotDialog, Test_constructor_and_destructor)
{
}

TEST_F(Test_StartAutoPilotDialog, Test_on_accountComboBox_currentIndexChanged)
{
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->count(), 2);
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->itemText(0), "Matreshka");
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->itemText(1), "Balalayka");
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->currentIndex(), 0);

    dialog->ui->anotherAccountComboBox->setCurrentIndex(1);
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->currentIndex(), 1);

    dialog->ui->accountComboBox->setCurrentIndex(1);

    ASSERT_EQ(dialog->ui->anotherAccountComboBox->count(), 2);
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->itemText(0), "Babushka");
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->itemText(1), "Balalayka");
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->currentIndex(), 0);

    dialog->ui->accountComboBox->setCurrentIndex(2);

    ASSERT_EQ(dialog->ui->anotherAccountComboBox->count(), 2);
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->itemText(0), "Babushka");
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->itemText(1), "Matreshka");
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->currentIndex(), 0);

    dialog->ui->accountComboBox->clear();

    ASSERT_EQ(dialog->ui->anotherAccountComboBox->count(), 1);
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->itemText(0), "No another account");
    ASSERT_EQ(dialog->ui->anotherAccountComboBox->currentIndex(), 0);
}

TEST_F(Test_StartAutoPilotDialog, Test_on_followRadioButton_toggled)
{
    ASSERT_EQ(dialog->ui->followWidget->isEnabled(), false);

    dialog->ui->followRadioButton->setChecked(true);
    ASSERT_EQ(dialog->ui->followWidget->isEnabled(), true);

    dialog->ui->internalRadioButton->setChecked(true);
    ASSERT_EQ(dialog->ui->followWidget->isEnabled(), false);
}

TEST_F(Test_StartAutoPilotDialog, Test_on_startButton_clicked)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartAutoPilotDialog/account"),        QVariant("Babushka")));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartAutoPilotDialog/follow"),         QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartAutoPilotDialog/anotherAccount"), QVariant("Matreshka")));
    // clang-format on

    dialog->ui->startButton->click();

    dialog->ui->followRadioButton->setChecked(true);
    dialog->ui->accountComboBox->clear();
    dialog->ui->accountComboBox->addItem("BLAH");

    EXPECT_CALL(
        *messageBoxUtilsMock,
        warning(
            dialog,
            QString("No another account"),
            QString("There is no any account to follow"),
            QMessageBox::StandardButtons(QMessageBox::Ok),
            QMessageBox::NoButton
        )
    );

    dialog->ui->startButton->click();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
