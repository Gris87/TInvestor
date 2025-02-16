#pragma once



#include "src/utils/messagebox/imessagebox.h"



class MessageBox : public IMessageBox
{
public:
    explicit MessageBox();
    ~MessageBox();

    MessageBox(const MessageBox& another)            = delete;
    MessageBox& operator=(const MessageBox& another) = delete;

    QMessageBox::StandardButton warning(
        QWidget*                     parent,
        const QString&               title,
        const QString&               text,
        QMessageBox::StandardButtons buttons       = QMessageBox::Ok,
        QMessageBox::StandardButton  defaultButton = QMessageBox::NoButton
    ) override;
};
