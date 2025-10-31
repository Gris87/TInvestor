#include "src/widgets/portfoliotreewidget/portfoliotreewidgetfactory.h"

#include <QDebug>

#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"



PortfolioTreeWidgetFactory::PortfolioTreeWidgetFactory() :
    IPortfolioTreeWidgetFactory()
{
    qDebug() << "Create PortfolioTreeWidgetFactory";
}

PortfolioTreeWidgetFactory::~PortfolioTreeWidgetFactory()
{
    qDebug() << "Destroy PortfolioTreeWidgetFactory";
}

IPortfolioTreeWidget* PortfolioTreeWidgetFactory::newInstance(
    IPortfolioTreeModelFactory* portfolioTreeModelFactory,
    IFileDialogFactory*         fileDialogFactory,
    IMessageBoxUtils*           messageBoxUtils,
    ISettingsEditor*            settingsEditor,
    bool                        autoPilot,
    QWidget*                    parent
) const
{
    return new PortfolioTreeWidget(
        portfolioTreeModelFactory, fileDialogFactory, messageBoxUtils, settingsEditor, autoPilot, parent
    );
}
