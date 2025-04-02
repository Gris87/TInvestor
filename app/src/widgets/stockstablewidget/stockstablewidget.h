#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class StocksTableWidget;
}



class StocksTableWidget : public IStocksTableWidget
{
    Q_OBJECT

public:
    explicit StocksTableWidget(ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~StocksTableWidget();

    StocksTableWidget(const StocksTableWidget& another)            = delete;
    StocksTableWidget& operator=(const StocksTableWidget& another) = delete;

    Ui::StocksTableWidget* ui;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    ISettingsEditor* mSettingsEditor;
};
