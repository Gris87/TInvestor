#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"



class IAuthDialogFactory
{
public:
    IAuthDialogFactory()
    {
    }
    virtual ~IAuthDialogFactory() = default;

    IAuthDialogFactory(const IAuthDialogFactory& another)            = delete;
    IAuthDialogFactory& operator=(const IAuthDialogFactory& another) = delete;

    virtual IAuthDialog* newInstance(QWidget* parent) = 0;
};
