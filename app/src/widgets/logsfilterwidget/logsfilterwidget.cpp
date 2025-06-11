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

void LogsFilterWidget::on_tickerLineEdit_textChanged(const QString& text)
{
    mFilter.ticker = text;

    filterChangeDelayTimer.start();
}
