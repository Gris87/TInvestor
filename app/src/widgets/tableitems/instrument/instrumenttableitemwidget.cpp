#include "src/widgets/tableitems/instrument/instrumenttableitemwidget.h"
#include "ui_instrumenttableitemwidget.h"

#include <QDebug>



InstrumentTableItemWidget::InstrumentTableItemWidget(IUserStorage* userStorage, QWidget* parent) :
    IInstrumentTableItemWidget(parent),
    ui(new Ui::InstrumentTableItemWidget),
    hoverTextWidget(),
    mUserStorage(userStorage),
    mForQualInvestorFlag(false),
    mFullText()
{
    qDebug() << "Create InstrumentTableItemWidget";

    ui->setupUi(this);

    ui->lockLabel->hide();
}

InstrumentTableItemWidget::~InstrumentTableItemWidget()
{
    qDebug() << "Destroy InstrumentTableItemWidget";

    delete hoverTextWidget;

    delete ui;
}

void InstrumentTableItemWidget::enterEvent(QEnterEvent* event)
{
    hoverTextWidget = new QLabel();
    hoverTextWidget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
    hoverTextWidget->setText(mFullText);

    const QPoint globalPos = ui->nameLabel->mapToGlobal(QPoint(0, 0));
    hoverTextWidget->setGeometry(globalPos.x(), globalPos.y(), hoverTextWidget->sizeHint().width(), ui->nameLabel->height());

    hoverTextWidget->show();
    ui->nameLabel->hide();

    IInstrumentTableItemWidget::enterEvent(event);
}

void InstrumentTableItemWidget::leaveEvent(QEvent* event)
{
    delete hoverTextWidget;
    hoverTextWidget = nullptr;

    ui->nameLabel->show();

    IInstrumentTableItemWidget::leaveEvent(event);
}

void InstrumentTableItemWidget::setIcon(const QIcon& icon)
{
    ui->logoLabel->setPixmap(icon.pixmap(ui->logoLabel->size()));
}

void InstrumentTableItemWidget::setQualInvestor(bool forQualInvestorFlag)
{
    mForQualInvestorFlag = forQualInvestorFlag;

    ui->lockLabel->setVisible(mForQualInvestorFlag && !mUserStorage->isQualified());
}

void InstrumentTableItemWidget::setText(const QString& text)
{
    ui->nameLabel->setText(text);
}

void InstrumentTableItemWidget::setFullText(const QString& text)
{
    mFullText = text;
}

bool InstrumentTableItemWidget::forQualInvestorFlag()
{
    return mForQualInvestorFlag;
}

QString InstrumentTableItemWidget::text()
{
    return ui->nameLabel->text();
}

QString InstrumentTableItemWidget::fullText()
{
    return mFullText;
}

bool InstrumentTableItemWidget::operator<(const QTableWidgetItem& another) const
{
    const QString anotherName = dynamic_cast<const InstrumentTableItemWidget*>(&another)->ui->nameLabel->text();

    return ui->nameLabel->text() < anotherName;
}
