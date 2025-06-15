#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"
#include "ui_startautopilotdialog.h"

#include <QDebug>



StartAutoPilotDialog::StartAutoPilotDialog(
    IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, ISettingsEditor* settingsEditor, QWidget* parent
) :
    IStartAutoPilotDialog(parent),
    ui(new Ui::StartAutoPilotDialog),
    mUserStorage(userStorage),
    mMessageBoxUtils(messageBoxUtils),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create StartAutoPilotDialog";

    ui->setupUi(this);
    fillAccounts();

    loadWindowState();
}

StartAutoPilotDialog::~StartAutoPilotDialog()
{
    qDebug() << "Destroy StartAutoPilotDialog";

    delete ui;
}

QString StartAutoPilotDialog::account() const
{
    return ui->accountComboBox->currentData().toString();
}

QString StartAutoPilotDialog::mode() const
{
    if (ui->internalRadioButton->isChecked())
    {
        return "INTERNAL";
    }

    if (ui->followRadioButton->isChecked())
    {
        return "FOLLOW";
    }

    return "VIEW";
}

QString StartAutoPilotDialog::anotherAccount() const
{
    return ui->anotherAccountComboBox->currentData().toString();
}

void StartAutoPilotDialog::on_accountComboBox_currentIndexChanged(int index) const
{
    ui->anotherAccountComboBox->clear();

    for (int i = 0; i < ui->accountComboBox->count(); ++i)
    {
        if (i != index)
        {
            ui->anotherAccountComboBox->addItem(ui->accountComboBox->itemText(i), ui->accountComboBox->itemData(i));
        }
    }

    if (ui->anotherAccountComboBox->count() == 0)
    {
        ui->anotherAccountComboBox->addItem(tr("No another account"), "");
    }
}

void StartAutoPilotDialog::on_followRadioButton_toggled(bool checked) const
{
    ui->followWidget->setEnabled(checked);
}

void StartAutoPilotDialog::on_startButton_clicked()
{
    if (ui->followRadioButton->isChecked() && ui->accountComboBox->count() == 1)
    {
        mMessageBoxUtils->warning(this, tr("No another account"), tr("There is no any account to follow"), QMessageBox::Ok);

        return;
    }

    saveWindowState();

    accept();
}

void StartAutoPilotDialog::fillAccounts()
{
    ui->accountComboBox->blockSignals(true);

    mUserStorage->lock();
    QList<Account> accounts = mUserStorage->getAccounts().values();
    mUserStorage->unlock();

    std::sort(accounts.begin(), accounts.end());

    for (const Account& account : std::as_const(accounts))
    {
        ui->accountComboBox->addItem(account.name, account.hash());
    }

    ui->accountComboBox->blockSignals(false);

    on_accountComboBox_currentIndexChanged(ui->accountComboBox->currentIndex());
}

void StartAutoPilotDialog::saveWindowState()
{
    qDebug() << "Saving window state";

    // clang-format off
    mSettingsEditor->setValue("StartAutoPilotDialog/account",        ui->accountComboBox->currentData());
    mSettingsEditor->setValue("StartAutoPilotDialog/anotherAccount", ui->anotherAccountComboBox->currentData());
    mSettingsEditor->setValue("StartAutoPilotDialog/mode",           mode());
    // clang-format on
}

void StartAutoPilotDialog::loadWindowState()
{
    qDebug() << "Loading window state";

    // clang-format off
    const QString account        = mSettingsEditor->value("StartAutoPilotDialog/account",        "").toString();
    const QString anotherAccount = mSettingsEditor->value("StartAutoPilotDialog/anotherAccount", "").toString();
    const QString mode           = mSettingsEditor->value("StartAutoPilotDialog/mode",           "INTERNAL").toString();
    // clang-format on

    ui->internalRadioButton->setChecked(mode == "INTERNAL");
    ui->followRadioButton->setChecked(mode == "FOLLOW");
    ui->viewRadioButton->setChecked(mode == "VIEW");

    for (int i = 0; i < ui->accountComboBox->count(); ++i)
    {
        if (ui->accountComboBox->itemData(i) == account)
        {
            ui->accountComboBox->setCurrentIndex(i);

            break;
        }
    }

    for (int i = 0; i < ui->anotherAccountComboBox->count(); ++i)
    {
        if (ui->anotherAccountComboBox->itemData(i) == anotherAccount)
        {
            ui->anotherAccountComboBox->setCurrentIndex(i);

            break;
        }
    }
}
