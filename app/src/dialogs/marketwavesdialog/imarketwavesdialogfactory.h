#pragma once



#include "src/dialogs/marketwavesdialog/imarketwavesdialog.h"



class IMarketWavesDialogFactory
{
public:
    IMarketWavesDialogFactory()
    {
    }
    virtual ~IMarketWavesDialogFactory() = default;

    IMarketWavesDialogFactory(const IMarketWavesDialogFactory& another)            = delete;
    IMarketWavesDialogFactory& operator=(const IMarketWavesDialogFactory& another) = delete;

    virtual std::shared_ptr<IMarketWavesDialog> newInstance(QWidget* parent) = 0;
};
