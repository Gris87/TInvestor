#include "src/widgets/tableitems/loglevel/logleveltableitemwidget.h"
#include "ui_logleveltableitemwidget.h"

#include <QDebug>



LogLevelTableItemWidget::LogLevelTableItemWidget(QWidget* parent) :
    ILogLevelTableItemWidget(parent),
    ui(new Ui::LogLevelTableItemWidget)
{
    qDebug() << "Create LogLevelTableItemWidget";

    ui->setupUi(this);
}

LogLevelTableItemWidget::~LogLevelTableItemWidget()
{
    qDebug() << "Destroy LogLevelTableItemWidget";

    delete ui;
}
