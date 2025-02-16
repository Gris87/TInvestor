#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"

#include "src/storage/user/iuserstorage.h"



namespace Ui
{
class AuthDialog;
}



class AuthDialog : public IAuthDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(IUserStorage* userStorage, QWidget* parent = nullptr);
    ~AuthDialog();

    AuthDialog(const AuthDialog& another)            = delete;
    AuthDialog& operator=(const AuthDialog& another) = delete;

    Ui::AuthDialog* ui;

private slots:
    void on_loginButton_clicked();
};
