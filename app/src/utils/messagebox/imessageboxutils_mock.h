#pragma once



#include "src/utils/messagebox/imessageboxutils.h"

#include <gmock/gmock.h>



class MessageBoxUtilsMock : public IMessageBoxUtils
{
public:
    MessageBoxUtilsMock() :
        IMessageBoxUtils()
    {
    }
    ~MessageBoxUtilsMock() override = default;

    MessageBoxUtilsMock(const MessageBoxUtilsMock& another)            = delete;
    MessageBoxUtilsMock& operator=(const MessageBoxUtilsMock& another) = delete;

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

    MOCK_METHOD(
        QMessageBox::StandardButton,
        question,
        (QWidget * parent,
         const QString&               title,
         const QString&               text,
         QMessageBox::StandardButtons buttons,
         QMessageBox::StandardButton  defaultButton),
        (override)
    );
};
