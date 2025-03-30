#include "src/widgets/tablerecord/items/stock/stocktableitemwidget.h"
#include "ui_stocktableitemwidget.h"

#include <QDebug>



StockTableItemWidget::StockTableItemWidget(IUserStorage* userStorage, QWidget* parent) :
    IStockTableItemWidget(parent),
    ui(new Ui::StockTableItemWidget),
    mUserStorage(userStorage),
    mHoverTextWidget()
{
    qDebug() << "Create StockTableItemWidget";

    ui->setupUi(this);
}

StockTableItemWidget::~StockTableItemWidget()
{
    qDebug() << "Destroy StockTableItemWidget";

    if (mHoverTextWidget != nullptr)
    {
        delete mHoverTextWidget;
    }

    delete ui;
}

void StockTableItemWidget::enterEvent(QEnterEvent* event)
{
    mHoverTextWidget = new QLabel();
    mHoverTextWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowTransparentForInput);
    mHoverTextWidget->setText(mFullText);

    QPoint globalPos = ui->nameLabel->mapToGlobal(QPoint(0, 0));
    mHoverTextWidget->setGeometry(globalPos.x(), globalPos.y(), mHoverTextWidget->sizeHint().width(), ui->nameLabel->height());

    mHoverTextWidget->show();
    ui->nameLabel->setVisible(false);

    IStockTableItemWidget::enterEvent(event);
}

void StockTableItemWidget::leaveEvent(QEvent* event)
{
    delete mHoverTextWidget;
    ui->nameLabel->setVisible(true);

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
