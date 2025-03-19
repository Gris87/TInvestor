#pragma once



#include "src/dialogs/marketwavesdialog/imarketwavesdialogfactory.h"



class MarketWavesDialogFactory : public IMarketWavesDialogFactory
{
public:
    MarketWavesDialogFactory();
    ~MarketWavesDialogFactory();

    MarketWavesDialogFactory(const MarketWavesDialogFactory& another)            = delete;
    MarketWavesDialogFactory& operator=(const MarketWavesDialogFactory& another) = delete;

    std::shared_ptr<IMarketWavesDialog> newInstance(QWidget* parent) override;
};
