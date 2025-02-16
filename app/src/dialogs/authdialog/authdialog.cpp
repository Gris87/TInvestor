#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QScreen>



AuthDialog::AuthDialog(IUserStorage* userStorage, IMessageBox* messageBox, QWidget* parent) :
    IAuthDialog(parent),
    ui(new Ui::AuthDialog),
    mMessageBox(messageBox)
{
    qDebug() << "Create AuthDialog";

    ui->setupUi(this);

    ui->tokenLineEdit->setText(userStorage->getToken());

    move(screen()->availableGeometry().center() - frameGeometry().center());
}

AuthDialog::~AuthDialog()
{
    qDebug() << "Destroy AuthDialog";

    delete ui;
}

void AuthDialog::on_loginButton_clicked()
{
    if (ui->tokenLineEdit->text() == "")
    {
        mMessageBox->warning(this, tr("Token is empty"), tr("You should provide token"), QMessageBox::Ok);

        return;
    }

    accept();
}
