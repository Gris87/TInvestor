#include "src/widgets/operationstablewidget/operationstablewidgetfactory.h"

#include <QDebug>

#include "src/widgets/operationstablewidget/operationstablewidget.h"



OperationsTableWidgetFactory::OperationsTableWidgetFactory() :
    IOperationsTableWidgetFactory()
{
    qDebug() << "Create OperationsTableWidgetFactory";
}

OperationsTableWidgetFactory::~OperationsTableWidgetFactory()
{
    qDebug() << "Destroy OperationsTableWidgetFactory";
}

IOperationsTableWidget* OperationsTableWidgetFactory::newInstance(
    IOperationsTableModelFactory*      operationsTableModelFactory,
    ILogosStorage*                     logosStorage,
    IOperationsTableRecordFactory*     operationsTableRecordFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IFileDialogFactory*                fileDialogFactory,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) const
{
    return new OperationsTableWidget(
        operationsTableModelFactory,
        logosStorage,
        operationsTableRecordFactory,
        instrumentTableItemWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialogFactory,
        settingsEditor,
        parent
    );
}
