#pragma once



#include <QFileDialog>



class IFileDialog
{
public:
    IFileDialog()          = default;
    virtual ~IFileDialog() = default;

    IFileDialog(const IFileDialog& another)            = delete;
    IFileDialog& operator=(const IFileDialog& another) = delete;

    virtual void setAcceptMode(QFileDialog::AcceptMode mode) = 0;
    virtual void selectFile(const QString& filename)         = 0;
    virtual int  exec()                                      = 0;

    [[nodiscard]]
    virtual QStringList selectedFiles() const = 0;
};
