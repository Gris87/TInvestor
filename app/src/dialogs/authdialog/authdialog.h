#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"

#include "src/storage/user/iuserstorage.h"
#include "src/utils/messagebox/imessagebox.h"



namespace Ui
{
class AuthDialog;
}



class AuthDialog : public IAuthDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(IUserStorage* userStorage, IMessageBox* messageBox, QWidget* parent = nullptr);
    ~AuthDialog();

    AuthDialog(const AuthDialog& another)            = delete;
    AuthDialog& operator=(const AuthDialog& another) = delete;

    QString getToken() override;

    Ui::AuthDialog* ui;

private:
    IMessageBox* mMessageBox;

private slots:
    void on_loginButton_clicked();
};
