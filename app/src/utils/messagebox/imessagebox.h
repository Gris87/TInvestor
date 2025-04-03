#pragma once



#include <QMessageBox>



class IMessageBoxUtils
{
public:
    IMessageBoxUtils()
    {
    }
    virtual ~IMessageBoxUtils() = default;

    IMessageBoxUtils(const IMessageBoxUtils& another)            = delete;
    IMessageBoxUtils& operator=(const IMessageBoxUtils& another) = delete;

    virtual QMessageBox::StandardButton warning(
        QWidget*                     parent,
        const QString&               title,
        const QString&               text,
        QMessageBox::StandardButtons buttons       = QMessageBox::Ok,
        QMessageBox::StandardButton  defaultButton = QMessageBox::NoButton
    ) = 0;

    virtual QMessageBox::StandardButton question(
        QWidget*                     parent,
        const QString&               title,
        const QString&               text,
        QMessageBox::StandardButtons buttons       = QMessageBox::Yes | QMessageBox::No,
        QMessageBox::StandardButton  defaultButton = QMessageBox::NoButton
    ) = 0;
};
