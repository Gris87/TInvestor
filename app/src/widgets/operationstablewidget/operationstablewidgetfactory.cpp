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
    IOperationsTableModelFactory* operationsTableModelFactory,
    IFileDialogFactory*           fileDialogFactory,
    IConfig*                      config,
    ISettingsEditor*              settingsEditor,
    QWidget*                      parent
) const
{
    return new OperationsTableWidget(operationsTableModelFactory, fileDialogFactory, config, settingsEditor, parent);
}
