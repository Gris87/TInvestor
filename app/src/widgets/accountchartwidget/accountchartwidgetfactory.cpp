#include "src/widgets/accountchartwidget/accountchartwidgetfactory.h"

#include <QDebug>

#include "src/widgets/accountchartwidget/accountchartwidget.h"



AccountChartWidgetFactory::AccountChartWidgetFactory() :
    IAccountChartWidgetFactory()
{
    qDebug() << "Create AccountChartWidgetFactory";
}

AccountChartWidgetFactory::~AccountChartWidgetFactory()
{
    qDebug() << "Destroy AccountChartWidgetFactory";
}

IAccountChartWidget* AccountChartWidgetFactory::newInstance(
    IFileDialogFactory* fileDialogFactory, ISettingsEditor* settingsEditor, QWidget* parent
) const
{
    return new AccountChartWidget(fileDialogFactory, settingsEditor, parent);
}
