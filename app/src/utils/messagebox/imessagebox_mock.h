#pragma once



#include "src/utils/messagebox/imessagebox.h"

#include <gmock/gmock.h>



class MessageBoxMock : public IMessageBox
{
public:
    MessageBoxMock() :
        IMessageBox()
    {
    }
    ~MessageBoxMock() override = default;

    MessageBoxMock(const MessageBoxMock& another)            = delete;
    MessageBoxMock& operator=(const MessageBoxMock& another) = delete;

    MOCK_METHOD(
        QMessageBox::StandardButton,
        warning,
        (QWidget * parent,
         const QString&               title,
         const QString&               text,
         QMessageBox::StandardButtons buttons,
         QMessageBox::StandardButton  defaultButton),
        (override)
    );
};
