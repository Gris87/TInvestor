#pragma once



#include <QDialog>



class IAuthDialog : public QDialog
{
public:
    explicit IAuthDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    virtual ~IAuthDialog() = default;

    IAuthDialog(const IAuthDialog& another)            = delete;
    IAuthDialog& operator=(const IAuthDialog& another) = delete;
};
