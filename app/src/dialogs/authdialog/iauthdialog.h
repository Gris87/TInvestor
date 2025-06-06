#pragma once



#include <QDialog>



class IAuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IAuthDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    ~IAuthDialog() override = default;

    IAuthDialog(const IAuthDialog& another)            = delete;
    IAuthDialog& operator=(const IAuthDialog& another) = delete;

    virtual QString getToken() = 0;
};
