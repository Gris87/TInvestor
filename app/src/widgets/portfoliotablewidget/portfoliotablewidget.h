#pragma once



#include "src/widgets/portfoliotablewidget/iportfoliotablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



namespace Ui
{
class PortfolioTableWidget;
}



class PortfolioTableWidget : public IPortfolioTableWidget
{
    Q_OBJECT

public:
    explicit PortfolioTableWidget(ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~PortfolioTableWidget() override;

    PortfolioTableWidget(const PortfolioTableWidget& another)            = delete;
    PortfolioTableWidget& operator=(const PortfolioTableWidget& another) = delete;

    Ui::PortfolioTableWidget* ui;

    void portfolioChanged(const Portfolio& portfolio) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    ISettingsEditor* mSettingsEditor;
};
