#pragma once



#include "src/widgets/operationstablewidget/ioperationstablewidget.h"

#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory.h"
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
        IOperationsTableModelFactory*      operationsTableModelFactory,
        ILogosStorage*                     logosStorage,
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent = nullptr
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
    void exportToExcel(const QString& path) const;

    IOperationsTableRecordFactory*     mOperationsTableRecordFactory;
    IInstrumentTableItemWidgetFactory* mInstrumentTableItemWidgetFactory;
    IUserStorage*                      mUserStorage;
    IInstrumentsStorage*               mInstrumentsStorage;
    IFileDialogFactory*                mFileDialogFactory;
    ISettingsEditor*                   mSettingsEditor;
    IOperationsTableModel*             mOperationsTableModel;
    QList<IOperationsTableRecord*>     mRecords;

public slots:
    void on_tableView_customContextMenuRequested(const QPoint& pos);
    void actionExportToExcelTriggered();
};
