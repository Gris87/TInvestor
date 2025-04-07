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
    virtual ~IStartAutoPilotDialog() = default;

    IStartAutoPilotDialog(const IStartAutoPilotDialog& another)            = delete;
    IStartAutoPilotDialog& operator=(const IStartAutoPilotDialog& another) = delete;
};
