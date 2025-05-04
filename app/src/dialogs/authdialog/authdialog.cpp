#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QScreen>
#include <QTimer>

#include "src/domain/user/user.h"



constexpr qint64 MS_IN_SECOND        = 1000LL;
constexpr qint64 ONE_MINUTE          = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR            = 60LL * ONE_MINUTE;
constexpr qint64 AUTO_LOGIN_INTERVAL = ONE_HOUR; // 1 hour



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

    if (ui->tokenLineEdit->text() != "")
    {
        QTimer::singleShot(AUTO_LOGIN_INTERVAL, this, SLOT(on_loginButton_clicked()));
    }

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
