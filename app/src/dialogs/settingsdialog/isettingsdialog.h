#pragma once



#include <QDialog>



class ISettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ISettingsDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    virtual ~ISettingsDialog() = default;

    ISettingsDialog(const ISettingsDialog& another)            = delete;
    ISettingsDialog& operator=(const ISettingsDialog& another) = delete;

    virtual void updateUiFromConfig() = 0;
};
