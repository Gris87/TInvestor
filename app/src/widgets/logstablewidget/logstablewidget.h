#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class LogsTableWidget;
}



class LogsTableWidget : public ILogsTableWidget
{
    Q_OBJECT

public:
    explicit LogsTableWidget(ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~LogsTableWidget();

    LogsTableWidget(const LogsTableWidget& another)            = delete;
    LogsTableWidget& operator=(const LogsTableWidget& another) = delete;

    Ui::LogsTableWidget* ui;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    ISettingsEditor* mSettingsEditor;
};
