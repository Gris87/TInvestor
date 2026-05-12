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
    dialog->ui->notificationsEnabledCheckBox->blockSignals(true);
    dialog->ui->filterSystemCheckBox->blockSignals(true);
    dialog->ui->filterPortfolioCheckBox->blockSignals(true);
    dialog->ui->filterHugeSellCheckBox->blockSignals(true);
    dialog->ui->filterDividendsCheckBox->blockSignals(true);
    dialog->ui->filterPulseNeutralCheckBox->blockSignals(true);
    dialog->ui->filterPulseBuyCheckBox->blockSignals(true);
    dialog->ui->filterPulseSellCheckBox->blockSignals(true);

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("yandex.ru"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(8041));
    EXPECT_CALL(*configMock, isNotificationsEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterSystem()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPortfolio()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterHugeSell()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterDividends()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPulseNeutral()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPulseBuy()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPulseSell()).WillOnce(Return(true));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),              true);
    ASSERT_EQ(dialog->ui->serverAddressLineEdit->text(),             "yandex.ru");
    ASSERT_EQ(dialog->ui->serverPortSpinBox->value(),                8041);
    ASSERT_EQ(dialog->ui->notificationsEnabledCheckBox->isChecked(), true);
    ASSERT_EQ(dialog->ui->filterSystemCheckBox->isChecked(),         true);
    ASSERT_EQ(dialog->ui->filterPortfolioCheckBox->isChecked(),      true);
    ASSERT_EQ(dialog->ui->filterHugeSellCheckBox->isChecked(),       true);
    ASSERT_EQ(dialog->ui->filterDividendsCheckBox->isChecked(),      true);
    ASSERT_EQ(dialog->ui->filterPulseNeutralCheckBox->isChecked(),   true);
    ASSERT_EQ(dialog->ui->filterPulseBuyCheckBox->isChecked(),       true);
    ASSERT_EQ(dialog->ui->filterPulseSellCheckBox->isChecked(),      true);
    // clang-format on

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("mail.ru"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(443));
    EXPECT_CALL(*configMock, isNotificationsEnabled()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isFilterSystem()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isFilterPortfolio()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isFilterHugeSell()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isFilterDividends()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isFilterPulseNeutral()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isFilterPulseBuy()).WillOnce(Return(false));
    EXPECT_CALL(*configMock, isFilterPulseSell()).WillOnce(Return(false));

    dialog->updateUiFromConfig();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),              false);
    ASSERT_EQ(dialog->ui->serverAddressLineEdit->text(),             "mail.ru");
    ASSERT_EQ(dialog->ui->serverPortSpinBox->value(),                443);
    ASSERT_EQ(dialog->ui->notificationsEnabledCheckBox->isChecked(), false);
    ASSERT_EQ(dialog->ui->filterSystemCheckBox->isChecked(),         false);
    ASSERT_EQ(dialog->ui->filterPortfolioCheckBox->isChecked(),      false);
    ASSERT_EQ(dialog->ui->filterHugeSellCheckBox->isChecked(),       false);
    ASSERT_EQ(dialog->ui->filterDividendsCheckBox->isChecked(),      false);
    ASSERT_EQ(dialog->ui->filterPulseNeutralCheckBox->isChecked(),   false);
    ASSERT_EQ(dialog->ui->filterPulseBuyCheckBox->isChecked(),       false);
    ASSERT_EQ(dialog->ui->filterPulseSellCheckBox->isChecked(),      false);
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

TEST_F(Test_SettingsDialog, Test_on_notificationsEnabledCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->notificationsEnabledCheckBox->blockSignals(true);
    dialog->ui->notificationsEnabledCheckBox->setChecked(false);
    dialog->ui->notificationsEnabledCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setNotificationsEnabled(true));
    dialog->ui->notificationsEnabledCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setNotificationsEnabled(false));
    dialog->ui->notificationsEnabledCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_filterSystemCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->filterSystemCheckBox->blockSignals(true);
    dialog->ui->filterSystemCheckBox->setChecked(false);
    dialog->ui->filterSystemCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setFilterSystem(true));
    dialog->ui->filterSystemCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setFilterSystem(false));
    dialog->ui->filterSystemCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_filterPortfolioCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->filterPortfolioCheckBox->blockSignals(true);
    dialog->ui->filterPortfolioCheckBox->setChecked(false);
    dialog->ui->filterPortfolioCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setFilterPortfolio(true));
    dialog->ui->filterPortfolioCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setFilterPortfolio(false));
    dialog->ui->filterPortfolioCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_filterHugeSellCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->filterHugeSellCheckBox->blockSignals(true);
    dialog->ui->filterHugeSellCheckBox->setChecked(false);
    dialog->ui->filterHugeSellCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setFilterHugeSell(true));
    dialog->ui->filterHugeSellCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setFilterHugeSell(false));
    dialog->ui->filterHugeSellCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_filterDividendsCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->filterDividendsCheckBox->blockSignals(true);
    dialog->ui->filterDividendsCheckBox->setChecked(false);
    dialog->ui->filterDividendsCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setFilterDividends(true));
    dialog->ui->filterDividendsCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setFilterDividends(false));
    dialog->ui->filterDividendsCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_filterPulseNeutralCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->filterPulseNeutralCheckBox->blockSignals(true);
    dialog->ui->filterPulseNeutralCheckBox->setChecked(false);
    dialog->ui->filterPulseNeutralCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setFilterPulseNeutral(true));
    dialog->ui->filterPulseNeutralCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setFilterPulseNeutral(false));
    dialog->ui->filterPulseNeutralCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_filterPulseBuyCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->filterPulseBuyCheckBox->blockSignals(true);
    dialog->ui->filterPulseBuyCheckBox->setChecked(false);
    dialog->ui->filterPulseBuyCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setFilterPulseBuy(true));
    dialog->ui->filterPulseBuyCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setFilterPulseBuy(false));
    dialog->ui->filterPulseBuyCheckBox->setChecked(false);
}

TEST_F(Test_SettingsDialog, Test_on_filterPulseSellCheckBox_checkStateChanged)
{
    const InSequence seq;

    dialog->ui->filterPulseSellCheckBox->blockSignals(true);
    dialog->ui->filterPulseSellCheckBox->setChecked(false);
    dialog->ui->filterPulseSellCheckBox->blockSignals(false);

    EXPECT_CALL(*configMock, setFilterPulseSell(true));
    dialog->ui->filterPulseSellCheckBox->setChecked(true);

    EXPECT_CALL(*configMock, setFilterPulseSell(false));
    dialog->ui->filterPulseSellCheckBox->setChecked(false);
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
    dialog->ui->notificationsEnabledCheckBox->blockSignals(true);
    dialog->ui->filterSystemCheckBox->blockSignals(true);
    dialog->ui->filterPortfolioCheckBox->blockSignals(true);
    dialog->ui->filterHugeSellCheckBox->blockSignals(true);
    dialog->ui->filterDividendsCheckBox->blockSignals(true);
    dialog->ui->filterPulseNeutralCheckBox->blockSignals(true);
    dialog->ui->filterPulseBuyCheckBox->blockSignals(true);
    dialog->ui->filterPulseSellCheckBox->blockSignals(true);

    EXPECT_CALL(*configMock, makeDefault());

    EXPECT_CALL(*configMock, isAutorun()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("yandex.ru"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(8041));
    EXPECT_CALL(*configMock, isNotificationsEnabled()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterSystem()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPortfolio()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterHugeSell()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterDividends()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPulseNeutral()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPulseBuy()).WillOnce(Return(true));
    EXPECT_CALL(*configMock, isFilterPulseSell()).WillOnce(Return(true));

    dialog->ui->defaultButton->click();

    // clang-format off
    ASSERT_EQ(dialog->ui->autorunCheckBox->isChecked(),              true);
    ASSERT_EQ(dialog->ui->serverAddressLineEdit->text(),             "yandex.ru");
    ASSERT_EQ(dialog->ui->serverPortSpinBox->value(),                8041);
    ASSERT_EQ(dialog->ui->notificationsEnabledCheckBox->isChecked(), true);
    ASSERT_EQ(dialog->ui->filterSystemCheckBox->isChecked(),         true);
    ASSERT_EQ(dialog->ui->filterPortfolioCheckBox->isChecked(),      true);
    ASSERT_EQ(dialog->ui->filterHugeSellCheckBox->isChecked(),       true);
    ASSERT_EQ(dialog->ui->filterDividendsCheckBox->isChecked(),      true);
    ASSERT_EQ(dialog->ui->filterPulseNeutralCheckBox->isChecked(),   true);
    ASSERT_EQ(dialog->ui->filterPulseBuyCheckBox->isChecked(),       true);
    ASSERT_EQ(dialog->ui->filterPulseSellCheckBox->isChecked(),      true);
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
