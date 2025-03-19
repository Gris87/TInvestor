#include "src/dialogs/marketwavesdialog/marketwavesdialogfactory.h"

#include <QDebug>

#include "src/dialogs/marketwavesdialog/marketwavesdialog.h"



MarketWavesDialogFactory::MarketWavesDialogFactory() :
    IMarketWavesDialogFactory()
{
    qDebug() << "Create MarketWavesDialogFactory";
}

MarketWavesDialogFactory::~MarketWavesDialogFactory()
{
    qDebug() << "Destroy MarketWavesDialogFactory";
}

std::shared_ptr<IMarketWavesDialog> MarketWavesDialogFactory::newInstance(QWidget* parent)
{
    return std::shared_ptr<IMarketWavesDialog>(new MarketWavesDialog(parent));
}
