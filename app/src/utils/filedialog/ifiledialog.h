#pragma once



#include <QFileDialog>



class IFileDialog
{
public:
    IFileDialog()          = default;
    virtual ~IFileDialog() = default;

    IFileDialog(const IFileDialog& another)            = delete;
    IFileDialog& operator=(const IFileDialog& another) = delete;

    virtual void setWindowTitle(const QString& title)        = 0;
    virtual void setAcceptMode(QFileDialog::AcceptMode mode) = 0;
};
