#pragma once



#include "src/utils/messagebox/imessagebox.h"



class MessageBoxUtils : public IMessageBoxUtils
{
public:
    explicit MessageBoxUtils();
    ~MessageBoxUtils();

    MessageBoxUtils(const MessageBoxUtils& another)            = delete;
    MessageBoxUtils& operator=(const MessageBoxUtils& another) = delete;

    QMessageBox::StandardButton warning(
        QWidget*                     parent,
        const QString&               title,
        const QString&               text,
        QMessageBox::StandardButtons buttons       = QMessageBox::Ok,
        QMessageBox::StandardButton  defaultButton = QMessageBox::NoButton
    ) override;

    QMessageBox::StandardButton question(
        QWidget*                     parent,
        const QString&               title,
        const QString&               text,
        QMessageBox::StandardButtons buttons       = QMessageBox::Yes | QMessageBox::No,
        QMessageBox::StandardButton  defaultButton = QMessageBox::NoButton
    ) override;
};
