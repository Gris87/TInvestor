#pragma once



#include "src/dialogs/marketwavesdialog/imarketwavesdialog.h"



namespace Ui
{
class MarketWavesDialog;
}



class MarketWavesDialog : public IMarketWavesDialog
{
    Q_OBJECT

public:
    explicit MarketWavesDialog(QWidget* parent = nullptr);
    ~MarketWavesDialog();

    MarketWavesDialog(const MarketWavesDialog& another)            = delete;
    MarketWavesDialog& operator=(const MarketWavesDialog& another) = delete;

    Ui::MarketWavesDialog* ui;
};
