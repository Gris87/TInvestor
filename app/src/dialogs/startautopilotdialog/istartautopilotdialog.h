#pragma once



#include <QDialog>



extern const char* const AUTO_PILOT_MODE_INTERNAL;
extern const char* const AUTO_PILOT_MODE_FOLLOW;
extern const char* const AUTO_PILOT_MODE_VIEW;



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

    [[nodiscard]]
    virtual QString mode() const = 0;

    [[nodiscard]]
    virtual QString account() const = 0;

    [[nodiscard]]
    virtual QString anotherAccount() const = 0;
};
