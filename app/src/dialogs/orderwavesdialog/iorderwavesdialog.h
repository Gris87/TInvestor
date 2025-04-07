#pragma once



#include <QDialog>



class IOrderWavesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IOrderWavesDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    virtual ~IOrderWavesDialog() = default;

    IOrderWavesDialog(const IOrderWavesDialog& another)            = delete;
    IOrderWavesDialog& operator=(const IOrderWavesDialog& another) = delete;
};
