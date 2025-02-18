#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QScreen>



AuthDialog::AuthDialog(IUserStorage* userStorage, IMessageBox* messageBox, QWidget* parent) :
    IAuthDialog(parent),
    ui(new Ui::AuthDialog),
    mMessageBox(messageBox),
    mTokenRegexp("t\\.[\\w_]{64}\\-[\\w_]{21}")
{
    qDebug() << "Create AuthDialog";

    ui->setupUi(this);

#ifndef USE_SANDBOX
    ui->tokenLineEdit->setText(userStorage->getToken());
#else
    Q_UNUSED(userStorage);
    ui->tokenLineEdit->setText(SANDBOX_TOKEN);
#endif

    move(screen()->availableGeometry().center() - frameGeometry().center());
}

AuthDialog::~AuthDialog()
{
    qDebug() << "Destroy AuthDialog";

    delete ui;
}

QString AuthDialog::getToken()
{
    return ui->tokenLineEdit->text();
}

void AuthDialog::on_loginButton_clicked()
{
    QRegularExpressionMatch match = mTokenRegexp.match(getToken());

    if (!match.hasMatch())
    {
        mMessageBox->warning(this, tr("Token is invalid"), tr("You should provide correct token"), QMessageBox::Ok);

        return;
    }

    accept();
}
