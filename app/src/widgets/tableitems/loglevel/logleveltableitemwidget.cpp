#include "src/widgets/tableitems/loglevel/logleveltableitemwidget.h"
#include "ui_logleveltableitemwidget.h"

#include <QDebug>



LogLevelTableItemWidget::LogLevelTableItemWidget(QWidget* parent) :
    ILogLevelTableItemWidget(parent),
    ui(new Ui::LogLevelTableItemWidget),
    mLogLevel()
{
    qDebug() << "Create LogLevelTableItemWidget";

    ui->setupUi(this);
}

LogLevelTableItemWidget::~LogLevelTableItemWidget()
{
    qDebug() << "Destroy LogLevelTableItemWidget";

    delete ui;
}

void LogLevelTableItemWidget::setLogLevel(LogLevel level)
{
    mLogLevel = level;

    const QIcon icon(QString(":/assets/images/levels/%1.png").arg(LOG_LEVEL_NAMES_LOWERCASE[level]));
    ui->iconLabel->setPixmap(icon.pixmap(ui->iconLabel->size()));
}

LogLevel LogLevelTableItemWidget::logLevel() const
{
    return mLogLevel;
}

bool LogLevelTableItemWidget::operator<(const QTableWidgetItem& another) const
{
    const LogLevel anotherLogLevel = dynamic_cast<const LogLevelTableItemWidget*>(&another)->mLogLevel;

    return mLogLevel < anotherLogLevel;
}
