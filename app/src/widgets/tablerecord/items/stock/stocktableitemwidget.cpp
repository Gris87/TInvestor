#include "src/widgets/tablerecord/items/stock/stocktableitemwidget.h"
#include "ui_stocktableitemwidget.h"

#include <QDebug>



StockTableItemWidget::StockTableItemWidget(IUserStorage* userStorage, QWidget* parent) :
    IStockTableItemWidget(parent),
    ui(new Ui::StockTableItemWidget),
    mUserStorage(userStorage)
{
    qDebug() << "Create StockTableItemWidget";

    ui->setupUi(this);
}

StockTableItemWidget::~StockTableItemWidget()
{
    qDebug() << "Destroy StockTableItemWidget";

    delete ui;
}

void StockTableItemWidget::setIcon(const QIcon& icon)
{
    ui->logoLabel->setPixmap(icon.pixmap(ui->logoLabel->size()));
}

void StockTableItemWidget::setQualInvestor(bool forQualInvestorFlag)
{
    mForQualInvestorFlag = forQualInvestorFlag;

    ui->lockLabel->setVisible(mForQualInvestorFlag && !mUserStorage->isQualified());
}

void StockTableItemWidget::setText(const QString& text)
{
    ui->nameLabel->setText(text);
}

void StockTableItemWidget::setFullText(const QString& text)
{
    ui->nameLabel->setToolTip(text);
}

bool StockTableItemWidget::forQualInvestorFlag()
{
    return mForQualInvestorFlag;
}

QString StockTableItemWidget::text()
{
    return ui->nameLabel->text();
}

QString StockTableItemWidget::fullText()
{
    return ui->nameLabel->toolTip();
}

bool StockTableItemWidget::operator<(const QTableWidgetItem& another) const
{
    QString anotherName = static_cast<const StockTableItemWidget*>(&another)->ui->nameLabel->text();

    return ui->nameLabel->text() < anotherName;
}
