#include "src/widgets/tableitems/stock/stocktableitemwidget.h"
#include "ui_stocktableitemwidget.h"

#include <QDebug>



StockTableItemWidget::StockTableItemWidget(IUserStorage* userStorage, QWidget* parent) :
    IStockTableItemWidget(parent),
    ui(new Ui::StockTableItemWidget),
    mUserStorage(userStorage),
    mForQualInvestorFlag(false),
    hoverTextWidget()
{
    qDebug() << "Create StockTableItemWidget";

    ui->setupUi(this);
}

StockTableItemWidget::~StockTableItemWidget()
{
    qDebug() << "Destroy StockTableItemWidget";

    if (hoverTextWidget != nullptr)
    {
        delete hoverTextWidget;
    }

    delete ui;
}

void StockTableItemWidget::enterEvent(QEnterEvent* event)
{
    hoverTextWidget = new QLabel();
    hoverTextWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowTransparentForInput);
    hoverTextWidget->setText(mFullText);

    QPoint globalPos = ui->nameLabel->mapToGlobal(QPoint(0, 0));
    hoverTextWidget->setGeometry(globalPos.x(), globalPos.y(), hoverTextWidget->sizeHint().width(), ui->nameLabel->height());

    hoverTextWidget->show();
    ui->nameLabel->hide();

    IStockTableItemWidget::enterEvent(event);
}

void StockTableItemWidget::leaveEvent(QEvent* event)
{
    delete hoverTextWidget;
    hoverTextWidget = nullptr;

    ui->nameLabel->show();

    IStockTableItemWidget::leaveEvent(event);
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
    mFullText = text;
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
    return mFullText;
}

bool StockTableItemWidget::operator<(const QTableWidgetItem& another) const
{
    QString anotherName = static_cast<const StockTableItemWidget*>(&another)->ui->nameLabel->text();

    return ui->nameLabel->text() < anotherName;
}
