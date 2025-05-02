#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QScreen>

#include "src/domain/user/user.h"



AuthDialog::AuthDialog(IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, QWidget* parent) :
    IAuthDialog(parent),
    ui(new Ui::AuthDialog),
    mMessageBoxUtils(messageBoxUtils),
    mTokenRegexp(R"(t\.[\w_\-]+)")
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
    const QRegularExpressionMatch match = mTokenRegexp.match(getToken());

    if (!match.hasMatch())
    {
        mMessageBoxUtils->warning(this, tr("Token is invalid"), tr("You should provide correct token"), QMessageBox::Ok);

        return;
    }

    accept();
}
