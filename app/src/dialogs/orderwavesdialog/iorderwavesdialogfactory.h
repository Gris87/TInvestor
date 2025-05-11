#pragma once



#include "src/dialogs/orderwavesdialog/iorderwavesdialog.h"

#include "src/domain/stock/stock.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"



class IOrderWavesDialogFactory
{
public:
    IOrderWavesDialogFactory()          = default;
    virtual ~IOrderWavesDialogFactory() = default;

    IOrderWavesDialogFactory(const IOrderWavesDialogFactory& another)            = delete;
    IOrderWavesDialogFactory& operator=(const IOrderWavesDialogFactory& another) = delete;

    virtual std::shared_ptr<IOrderWavesDialog> newInstance(
        IOrderWavesWidgetFactory* orderWavesWidgetFactory,
        IOrderBookThread*         orderBookThread,
        Stock*                    stock,
        int                       precision,
        QWidget*                  parent
    ) const = 0;
};
