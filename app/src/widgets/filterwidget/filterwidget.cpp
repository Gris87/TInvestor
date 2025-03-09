#include "src/widgets/filterwidget/filterwidget.h"
#include "ui_filterwidget.h"

#include <QDebug>



FilterWidget::FilterWidget(QWidget* parent) :
    IFilterWidget(parent),
    ui(new Ui::FilterWidget)
{
    qDebug() << "Create FilterWidget";

    ui->setupUi(this);
}

FilterWidget::~FilterWidget()
{
    qDebug() << "Destroy FilterWidget";

    delete ui;
}
