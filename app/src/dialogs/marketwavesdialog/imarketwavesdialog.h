#pragma once



#include <QDialog>



class IMarketWavesDialog : public QDialog
{
public:
    explicit IMarketWavesDialog(QWidget* parent = nullptr) :
        QDialog(parent)
    {
    }
    virtual ~IMarketWavesDialog() = default;

    IMarketWavesDialog(const IMarketWavesDialog& another)            = delete;
    IMarketWavesDialog& operator=(const IMarketWavesDialog& another) = delete;
};
