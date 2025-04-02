#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class OperationsTableWidget;
}



class OperationsTableWidget : public IOperationsTableWidget
{
    Q_OBJECT

public:
    explicit OperationsTableWidget(ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~OperationsTableWidget();

    OperationsTableWidget(const OperationsTableWidget& another)            = delete;
    OperationsTableWidget& operator=(const OperationsTableWidget& another) = delete;

    Ui::OperationsTableWidget* ui;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    ISettingsEditor* mSettingsEditor;
};
