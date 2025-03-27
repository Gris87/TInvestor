#include "src/widgets/tablerecord/items/stock/stocktableitemwidget.h"
#include "ui_stocktableitemwidget.h"

#include <QDebug>



StockTableItemWidget::StockTableItemWidget(QWidget* parent) :
    IStockTableItemWidget(parent),
    ui(new Ui::StockTableItemWidget)
{
    qDebug() << "Create StockTableItemWidget";

    ui->setupUi(this);
}

StockTableItemWidget::~StockTableItemWidget()
{
    qDebug() << "Destroy StockTableItemWidget";

    delete ui;
}
