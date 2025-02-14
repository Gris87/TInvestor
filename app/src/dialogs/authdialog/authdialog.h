#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"



namespace Ui
{
class AuthDialog;
}



class AuthDialog : public IAuthDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget* parent = nullptr);
    ~AuthDialog();

    AuthDialog(const AuthDialog& another)            = delete;
    AuthDialog& operator=(const AuthDialog& another) = delete;

    Ui::AuthDialog* ui;
};
