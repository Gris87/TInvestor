#include "src/widgets/instrumentwidget/instrumentwidget.h"
#include "ui_instrumentwidget.h"

#include <QDebug>



InstrumentWidget::InstrumentWidget(IUserStorage* userStorage, QWidget* parent) :
    IInstrumentWidget(parent),
    ui(new Ui::InstrumentWidget),
    hoverTextWidget(),
    mUserStorage(userStorage),
    mForQualInvestorFlag(false),
    mFullText()
{
    qDebug() << "Create InstrumentWidget";

    ui->setupUi(this);

    ui->lockLabel->hide();
}

InstrumentWidget::~InstrumentWidget()
{
    qDebug() << "Destroy InstrumentWidget";

    delete hoverTextWidget;

    delete ui;
}

void InstrumentWidget::enterEvent(QEnterEvent* event)
{
    if (mFullText != "")
    {
        hoverTextWidget = new QLabel();
        hoverTextWidget->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
        hoverTextWidget->setText(mFullText);

        const QPoint globalPos = ui->nameLabel->mapToGlobal(QPoint(0, 0));
        hoverTextWidget->setGeometry(globalPos.x(), globalPos.y(), hoverTextWidget->sizeHint().width(), ui->nameLabel->height());

        hoverTextWidget->show();
        ui->nameLabel->hide();
    }

    IInstrumentWidget::enterEvent(event);
}

void InstrumentWidget::leaveEvent(QEvent* event)
{
    if (hoverTextWidget != nullptr)
    {
        delete hoverTextWidget;
        hoverTextWidget = nullptr;

        ui->nameLabel->show();
    }

    IInstrumentWidget::leaveEvent(event);
}

void InstrumentWidget::setInstrumentLogo(const QIcon& icon)
{
    ui->logoLabel->setPixmap(icon.pixmap(ui->logoLabel->size()));
}

void InstrumentWidget::setQualInvestor(bool forQualInvestorFlag)
{
    mForQualInvestorFlag = forQualInvestorFlag;

    ui->lockLabel->setVisible(mForQualInvestorFlag && !mUserStorage->isQualified());
}

void InstrumentWidget::setTicker(const QString& text)
{
    ui->nameLabel->setText(text);
}

void InstrumentWidget::setFullText(const QString& text)
{
    mFullText = text;
}

bool InstrumentWidget::forQualInvestorFlag() const
{
    return mForQualInvestorFlag;
}

QString InstrumentWidget::ticker() const
{
    return ui->nameLabel->text();
}

QString InstrumentWidget::fullText() const
{
    return mFullText;
}
