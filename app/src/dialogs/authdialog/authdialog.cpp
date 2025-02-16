#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QScreen>



AuthDialog::AuthDialog(IUserStorage* userStorage, QWidget* parent) :
    IAuthDialog(parent),
    ui(new Ui::AuthDialog)
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
        QMessageBox::warning(this, tr("Token is empty"), tr("You should provide token"), QMessageBox::Ok);

        return;
    }

    accept();
}
