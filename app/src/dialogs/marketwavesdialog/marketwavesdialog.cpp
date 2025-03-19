#include "src/dialogs/marketwavesdialog/marketwavesdialog.h"
#include "ui_marketwavesdialog.h"

#include <QDebug>



MarketWavesDialog::MarketWavesDialog(QWidget* parent) :
    IMarketWavesDialog(parent),
    ui(new Ui::MarketWavesDialog)
{
    qDebug() << "Create MarketWavesDialog";

    ui->setupUi(this);

    setWindowFlags(
        Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint |
        Qt::WindowCloseButtonHint
    );
}

MarketWavesDialog::~MarketWavesDialog()
{
    qDebug() << "Destroy MarketWavesDialog";

    delete ui;
}
