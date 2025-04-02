#include "src/widgets/portfoliotablewidget/portfoliotablewidget.h"
#include "ui_portfoliotablewidget.h"

#include <QDebug>



PortfolioTableWidget::PortfolioTableWidget(ISettingsEditor* settingsEditor, QWidget* parent) :
    IPortfolioTableWidget(parent),
    ui(new Ui::PortfolioTableWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create PortfolioTableWidget";

    ui->setupUi(this);
}

PortfolioTableWidget::~PortfolioTableWidget()
{
    qDebug() << "Destroy PortfolioTableWidget";

    delete ui;
}

void PortfolioTableWidget::saveWindowState(const QString& /*type*/)
{
}

void PortfolioTableWidget::loadWindowState(const QString& /*type*/)
{
}
