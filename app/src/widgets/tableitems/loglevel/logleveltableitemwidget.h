#pragma once



#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidget.h"



namespace Ui
{
class LogLevelTableItemWidget;
}



class LogLevelTableItemWidget : public ILogLevelTableItemWidget
{
    Q_OBJECT

public:
    explicit LogLevelTableItemWidget(QWidget* parent = nullptr);
    ~LogLevelTableItemWidget() override;

    LogLevelTableItemWidget(const LogLevelTableItemWidget& another)            = delete;
    LogLevelTableItemWidget& operator=(const LogLevelTableItemWidget& another) = delete;

    Ui::LogLevelTableItemWidget* ui;

    void     setLogLevel(LogLevel level) override;
    LogLevel logLevel() const override;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    LogLevel mLogLevel;
};
