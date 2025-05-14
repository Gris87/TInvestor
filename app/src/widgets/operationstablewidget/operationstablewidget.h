#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecordfactory.h"



namespace Ui
{
class OperationsTableWidget;
}



class OperationsTableWidget : public IOperationsTableWidget
{
    Q_OBJECT

public:
    explicit OperationsTableWidget(
        IOperationsTableRecordFactory* operationsTableRecordFactory,
        IStockTableItemWidgetFactory*  stockTableItemWidgetFactory,
        IUserStorage*                  userStorage,
        IInstrumentsStorage*           instrumentsStorage,
        ISettingsEditor*               settingsEditor,
        QWidget*                       parent = nullptr
    );
    ~OperationsTableWidget() override;

    OperationsTableWidget(const OperationsTableWidget& another)            = delete;
    OperationsTableWidget& operator=(const OperationsTableWidget& another) = delete;

    Ui::OperationsTableWidget* ui;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    IOperationsTableRecordFactory* mOperationsTableRecordFactory;
    IStockTableItemWidgetFactory*  mStockTableItemWidgetFactory;
    IUserStorage*                  mUserStorage;
    IInstrumentsStorage*           mInstrumentsStorage;
    ISettingsEditor*               mSettingsEditor;
    QList<IOperationsTableRecord*> mTableRecords;
};
