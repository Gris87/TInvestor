#pragma once



#include <QMessageBox>



class IMessageBox
{
public:
    IMessageBox()
    {
    }
    virtual ~IMessageBox() = default;

    IMessageBox(const IMessageBox& another)            = delete;
    IMessageBox& operator=(const IMessageBox& another) = delete;

    virtual QMessageBox::StandardButton warning(
        QWidget*                     parent,
        const QString&               title,
        const QString&               text,
        QMessageBox::StandardButtons buttons       = QMessageBox::Ok,
        QMessageBox::StandardButton  defaultButton = QMessageBox::NoButton
    ) = 0;
};
