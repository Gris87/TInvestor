#include "src/widgets/logsfilterwidget/logsfilterwidget.h"
#include "ui_logsfilterwidget.h"

#include <QDebug>



constexpr qint64 MS_IN_SECOND        = 1000LL;
constexpr qint64 FILTER_CHANGE_DELAY = MS_IN_SECOND; // 1 second



LogsFilterWidget::LogsFilterWidget(QWidget* parent) :
    ILogsFilterWidget(parent),
    ui(new Ui::LogsFilterWidget),
    mFilter()
{
    qDebug() << "Create LogsFilterWidget";

    ui->setupUi(this);

    filterChangeDelayTimer.setInterval(FILTER_CHANGE_DELAY);

    // clang-format off
    connect(&filterChangeDelayTimer, SIGNAL(timeout()), this, SLOT(filterChangeDelayTimerTicked()));
    // clang-format on
}

LogsFilterWidget::~LogsFilterWidget()
{
    qDebug() << "Destroy LogsFilterWidget";

    delete ui;
}

const LogFilter& LogsFilterWidget::getFilter()
{
    return mFilter;
}

void LogsFilterWidget::filterChangeDelayTimerTicked()
{
    filterChangeDelayTimer.stop();

    emit filterChanged(mFilter);
}

void LogsFilterWidget::on_verboseButton_clicked()
{
    if (mFilter.level != LOG_LEVEL_VERBOSE)
    {
        mFilter.level = LOG_LEVEL_VERBOSE;
        emit filterChanged(mFilter);
    }

    ui->verboseButton->setChecked(true);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(false);
}

void LogsFilterWidget::on_debugButton_clicked()
{
    if (mFilter.level != LOG_LEVEL_DEBUG)
    {
        mFilter.level = LOG_LEVEL_DEBUG;
        emit filterChanged(mFilter);
    }

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(true);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(false);
}

void LogsFilterWidget::on_infoButton_clicked()
{
    if (mFilter.level != LOG_LEVEL_INFO)
    {
        mFilter.level = LOG_LEVEL_INFO;
        emit filterChanged(mFilter);
    }

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(true);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(false);
}

void LogsFilterWidget::on_warningButton_clicked()
{
    if (mFilter.level != LOG_LEVEL_WARNING)
    {
        mFilter.level = LOG_LEVEL_WARNING;
        emit filterChanged(mFilter);
    }

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(true);
    ui->errorButton->setChecked(false);
}

void LogsFilterWidget::on_errorButton_clicked()
{
    if (mFilter.level != LOG_LEVEL_ERROR)
    {
        mFilter.level = LOG_LEVEL_ERROR;
        emit filterChanged(mFilter);
    }

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(true);
}

void LogsFilterWidget::on_tickerLineEdit_textChanged(const QString& text)
{
    mFilter.ticker = text;

    filterChangeDelayTimer.start();
}
