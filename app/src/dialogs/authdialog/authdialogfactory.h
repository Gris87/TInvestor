#pragma once



#include "src/dialogs/authdialog/iauthdialogfactory.h"



class AuthDialogFactory : public IAuthDialogFactory
{
public:
    AuthDialogFactory();
    ~AuthDialogFactory();

    AuthDialogFactory(const AuthDialogFactory& another)            = delete;
    AuthDialogFactory& operator=(const AuthDialogFactory& another) = delete;

    IAuthDialog* newInstance(IUserStorage* userStorage, IMessageBox* messageBox, QWidget* parent) override;
};
