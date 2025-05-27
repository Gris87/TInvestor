#pragma once



#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"



namespace Ui
{
class PortfolioTreeWidget;
}



class PortfolioTreeWidget : public IPortfolioTreeWidget
{
    Q_OBJECT

public:
    explicit PortfolioTreeWidget(ISettingsEditor* settingsEditor, QWidget* parent = nullptr);
    ~PortfolioTreeWidget() override;

    PortfolioTreeWidget(const PortfolioTreeWidget& another)            = delete;
    PortfolioTreeWidget& operator=(const PortfolioTreeWidget& another) = delete;

    Ui::PortfolioTreeWidget* ui;

    void portfolioChanged(const Portfolio& portfolio) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    ISettingsEditor* mSettingsEditor;
};
