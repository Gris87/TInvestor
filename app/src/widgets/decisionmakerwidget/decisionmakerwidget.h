#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory.h"



namespace Ui
{
class DecisionMakerWidget;
}



class DecisionMakerWidget : public IDecisionMakerWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerWidget(
        IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
        IAccountChartWidgetFactory*        accountChartWidgetFactory,
        ILogsFilterWidgetFactory*          logsFilterWidgetFactory,
        ILogsTableWidgetFactory*           logsTableWidgetFactory,
        IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        ILogsTableModelFactory*            logsTableModelFactory,
        IPortfolioTreeRecordFactory*       portfolioTreeRecordFactory,
        IInstrumentWidgetFactory*          instrumentWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        ILogosStorage*                     logosStorage,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent = nullptr
    );
    ~DecisionMakerWidget() override;

    DecisionMakerWidget(const DecisionMakerWidget& another)            = delete;
    DecisionMakerWidget& operator=(const DecisionMakerWidget& another) = delete;

    void setAccountName(const QString& name) override;

    void showSpinners() override;
    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;
    void logsRead(const QList<LogEntry>& entries) override;
    void logAdded(const LogEntry& entry) override;
    void portfolioChanged(const Portfolio& portfolio) override;
    void lastPriceChanged(const QString& instrumentId, float price) override;
    void updateLastPrices() override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

    Ui::DecisionMakerWidget* ui;

private:
    IOperationsTableWidget* mOperationsTableWidget;
    IAccountChartWidget*    mAccountChartWidget;
    ILogsFilterWidget*      mLogsFilterWidget;
    ILogsTableWidget*       mLogsTableWidget;
    IPortfolioTreeWidget*   mPortfolioTreeWidget;
    ISettingsEditor*        mSettingsEditor;

public slots:
    void logFilterChanged(const LogFilter& filter);

private slots:
    void on_yieldButton_clicked();
    void on_monthlyYieldButton_clicked();
    void on_remainedMoneyButton_clicked();
    void on_totalMoneyButton_clicked();
};
