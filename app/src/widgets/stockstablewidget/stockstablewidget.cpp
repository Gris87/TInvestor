#include "src/widgets/stockstablewidget/stockstablewidget.h"
#include "ui_stockstablewidget.h"

#include <QDebug>



StocksTableWidget::StocksTableWidget(ISettingsEditor* settingsEditor, QWidget* parent) :
    IStocksTableWidget(parent),
    ui(new Ui::StocksTableWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create StocksTableWidget";

    ui->setupUi(this);
}

StocksTableWidget::~StocksTableWidget()
{
    qDebug() << "Destroy StocksTableWidget";

    delete ui;
}

void StocksTableWidget::saveWindowState(const QString& /*type*/)
{
}

void StocksTableWidget::loadWindowState(const QString& /*type*/)
{
}
