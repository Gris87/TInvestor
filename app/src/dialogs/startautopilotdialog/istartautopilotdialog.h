#pragma once



#include <QDialog>



class IStartAutoPilotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IStartAutoPilotDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    ~IStartAutoPilotDialog() override = default;

    IStartAutoPilotDialog(const IStartAutoPilotDialog& another)            = delete;
    IStartAutoPilotDialog& operator=(const IStartAutoPilotDialog& another) = delete;

    virtual QString account() const        = 0;
    virtual QString mode() const           = 0;
    virtual QString anotherAccount() const = 0;
};
