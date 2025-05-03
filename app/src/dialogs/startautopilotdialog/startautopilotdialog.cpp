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

    ui->accountComboBox->addItems(getAccounts());

    loadWindowState();
}

StartAutoPilotDialog::~StartAutoPilotDialog()
{
    qDebug() << "Destroy StartAutoPilotDialog";

    delete ui;
}

void StartAutoPilotDialog::on_accountComboBox_currentIndexChanged(int index) const
{
    QStringList items;

    for (int i = 0; i < ui->accountComboBox->count(); ++i)
    {
        if (i != index)
        {
            items.append(ui->accountComboBox->itemText(i));
        }
    }

    if (items.isEmpty())
    {
        items.append(tr("No another account"));
    }

    ui->anotherAccountComboBox->clear();
    ui->anotherAccountComboBox->addItems(items);
    ui->anotherAccountComboBox->setCurrentIndex(0);
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

QStringList StartAutoPilotDialog::getAccounts()
{
    QStringList res;

    const QMutexLocker    lock(mUserStorage->getMutex());
    const QList<Account>& accounts = mUserStorage->getAccounts();

    res.resizeForOverwrite(accounts.size());

    QString* resData = res.data();

    for (int i = 0; i < accounts.size(); ++i)
    {
        resData[i] = accounts.at(i).name;
    }

    return res;
}

void StartAutoPilotDialog::saveWindowState()
{
    qDebug() << "Saving window state";

    // clang-format off
    mSettingsEditor->setValue("StartAutoPilotDialog/account",        ui->accountComboBox->currentText());
    mSettingsEditor->setValue("StartAutoPilotDialog/follow",         ui->followRadioButton->isChecked());
    mSettingsEditor->setValue("StartAutoPilotDialog/anotherAccount", ui->anotherAccountComboBox->currentText());
    // clang-format on
}

void StartAutoPilotDialog::loadWindowState()
{
    qDebug() << "Loading window state";

    // clang-format off
    ui->accountComboBox->setCurrentText(mSettingsEditor->value("StartAutoPilotDialog/account",               "").toString());
    ui->followRadioButton->setChecked(mSettingsEditor->value("StartAutoPilotDialog/follow",                  false).toBool());
    ui->anotherAccountComboBox->setCurrentText(mSettingsEditor->value("StartAutoPilotDialog/anotherAccount", "").toString());
    // clang-format on
}
