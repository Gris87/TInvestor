#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"

#include <QRegularExpression>

#include "src/storage/user/iuserstorage.h"
#include "src/utils/messagebox/imessagebox.h"



#define SANDBOX_TOKEN "t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ"



namespace Ui
{
class AuthDialog;
}



class AuthDialog : public IAuthDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(IUserStorage* userStorage, IMessageBoxUtils* messageBoxUtils, QWidget* parent = nullptr);
    ~AuthDialog();

    AuthDialog(const AuthDialog& another)            = delete;
    AuthDialog& operator=(const AuthDialog& another) = delete;

    QString getToken() override;

    Ui::AuthDialog* ui;

private:
    IMessageBoxUtils*  mMessageBoxUtils;
    QRegularExpression mTokenRegexp;

private slots:
    void on_loginButton_clicked();
};
