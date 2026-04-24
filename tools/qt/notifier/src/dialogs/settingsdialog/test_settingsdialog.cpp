#include "src/dialogs/settingsdialog/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_SettingsDialog : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock = new StrictMock<ConfigMock>();

        dialog = new SettingsDialog(configMock);
    }

    void TearDown() override
    {
        delete dialog;
        delete configMock;
    }

    SettingsDialog*         dialog;
    StrictMock<ConfigMock>* configMock;
};



TEST_F(Test_SettingsDialog, Test_constructor_and_destructor)
{
}

TEST_F(Test_SettingsDialog, Test_updateUiFromConfig)
{
    const InSequence seq;

    dialog->ui->autorunCheckBox->blockSignals(true);
    dialog->ui->serverAddressLineEdit->blockSignals(true);
    dialog->ui->serverPortSpinBox->blockSignals(true);

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("yandex.ru"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(8041));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),  true);
    ASSERT_EQ(dialog->ui->serverAddressLineEdit->text(), "yandex.ru");
    ASSERT_EQ(dialog->ui->serverPortSpinBox->value(),    8041);
    // clang-format on

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("mail.ru"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(443));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),  false);
    ASSERT_EQ(dialog->ui->serverAddressLineEdit->text(), "mail.ru");
    ASSERT_EQ(dialog->ui->serverPortSpinBox->value(),    443);
    // clang-format on
}

TEST_F(Test_SettingsDialog, Test_on_autorunCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->autorunCheckBox->blockSignals(true);
    dialog->ui->autorunCheckBox->setChecked(false);
    dialog->ui->autorunCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setAutorun(true));
    dialog->ui->autorunCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setAutorun(false));
    dialog->ui->autorunCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_serverAddressLineEdit_textChanged)
{
    const InSequence seq;

    dialog->ui->serverAddressLineEdit->blockSignals(true);
    dialog->ui->serverAddressLineEdit->setText("localhost");
    dialog->ui->serverAddressLineEdit->blockSignals(false);

    EXPECT_CALL(*configMock, setServerAddress(QString("yandex.ru")));
    dialog->ui->serverAddressLineEdit->setText("yandex.ru");

    EXPECT_CALL(*configMock, setServerAddress(QString("mail.ru")));
    dialog->ui->serverAddressLineEdit->setText("mail.ru");
}

TEST_F(Test_SettingsDialog, Test_on_serverPortSpinBox_valueChanged)
{
    const InSequence seq;

    dialog->ui->serverPortSpinBox->blockSignals(true);
    dialog->ui->serverPortSpinBox->setValue(1);
    dialog->ui->serverPortSpinBox->blockSignals(false);

    EXPECT_CALL(*configMock, setServerPort(2));
    dialog->ui->serverPortSpinBox->setValue(2);

    EXPECT_CALL(*configMock, setServerPort(3));
    dialog->ui->serverPortSpinBox->setValue(3);
}

TEST_F(Test_SettingsDialog, Test_on_okButton_clicked)
{
    ASSERT_EQ(dialog->result(), 0);
    dialog->ui->okButton->click();
    ASSERT_EQ(dialog->result(), QDialog::Accepted);
}

TEST_F(Test_SettingsDialog, Test_on_cancelButton_clicked)
{
    ASSERT_EQ(dialog->result(), 0);
    dialog->ui->cancelButton->click();
    ASSERT_EQ(dialog->result(), QDialog::Rejected);
}

TEST_F(Test_SettingsDialog, Test_on_defaultButton_clicked)
{
    const InSequence seq;

    dialog->ui->autorunCheckBox->blockSignals(true);
    dialog->ui->serverAddressLineEdit->blockSignals(true);
    dialog->ui->serverPortSpinBox->blockSignals(true);

    EXPECT_CALL(*configMock, makeDefault());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("yandex.ru"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(8041));

    dialog->ui->defaultButton->click();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),  true);
    ASSERT_EQ(dialog->ui->serverAddressLineEdit->text(), "yandex.ru");
    ASSERT_EQ(dialog->ui->serverPortSpinBox->value(),    8041);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
