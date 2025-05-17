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
    IOperationsTableRecordFactory*     operationsTableRecordFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IFileDialog*                       fileDialog,
    IMessageBoxUtils*                  mMessageBoxUtils,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) const
{
    return new OperationsTableWidget(
        operationsTableRecordFactory,
        instrumentTableItemWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialog,
        mMessageBoxUtils,
        settingsEditor,
        parent
    );
}
