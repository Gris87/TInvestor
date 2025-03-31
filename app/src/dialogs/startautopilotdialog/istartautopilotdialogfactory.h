#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialog.h"



class IStartAutoPilotDialogFactory
{
public:
    IStartAutoPilotDialogFactory()
    {
    }
    virtual ~IStartAutoPilotDialogFactory() = default;

    IStartAutoPilotDialogFactory(const IStartAutoPilotDialogFactory& another)            = delete;
    IStartAutoPilotDialogFactory& operator=(const IStartAutoPilotDialogFactory& another) = delete;

    virtual std::shared_ptr<IStartAutoPilotDialog> newInstance(QWidget* parent) = 0;
};
