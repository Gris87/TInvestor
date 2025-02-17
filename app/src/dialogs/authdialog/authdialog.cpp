#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QScreen>



#define SANDBOX_TOKEN "t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ"



AuthDialog::AuthDialog(IUserStorage* userStorage, IMessageBox* messageBox, QWidget* parent) :
    IAuthDialog(parent),
    ui(new Ui::AuthDialog),
    mMessageBox(messageBox),
    mTokenRegexp("t\\.\\w{64}\\-\\w{21}")
{
    qDebug() << "Create AuthDialog";

    ui->setupUi(this);

#ifdef USE_SANDBOX
    Q_UNUSED(userStorage);
    ui->tokenLineEdit->setText(SANDBOX_TOKEN);
#else
    ui->tokenLineEdit->setText(userStorage->getToken());
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
