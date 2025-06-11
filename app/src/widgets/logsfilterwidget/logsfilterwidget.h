#pragma once



#include "src/widgets/logsfilterwidget/ilogsfilterwidget.h"

#include <QTimer>



namespace Ui
{
class LogsFilterWidget;
}



class LogsFilterWidget : public ILogsFilterWidget
{
    Q_OBJECT

public:
    explicit LogsFilterWidget(QWidget* parent = nullptr);
    ~LogsFilterWidget() override;

    LogsFilterWidget(const LogsFilterWidget& another)            = delete;
    LogsFilterWidget& operator=(const LogsFilterWidget& another) = delete;

    Ui::LogsFilterWidget* ui;

    QTimer filterChangeDelayTimer;

    const LogFilter& getFilter() override;

private:
    LogFilter mFilter;

public slots:
    void filterChangeDelayTimerTicked();

private slots:
    void on_verboseButton_clicked();
    void on_debugButton_clicked();
    void on_infoButton_clicked();
    void on_warningButton_clicked();
    void on_errorButton_clicked();
    void on_tickerLineEdit_textChanged(const QString& text);
};
