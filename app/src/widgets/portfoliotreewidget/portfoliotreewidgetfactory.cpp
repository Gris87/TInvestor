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
    IPortfolioTreeModelFactory*  portfolioTreeModelFactory,
    ILogosStorage*               logosStorage,
    IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
    IInstrumentWidgetFactory*    instrumentWidgetFactory,
    IUserStorage*                userStorage,
    IInstrumentsStorage*         instrumentsStorage,
    IFileDialogFactory*          fileDialogFactory,
    ISettingsEditor*             settingsEditor,
    QWidget*                     parent
) const
{
    return new PortfolioTreeWidget(
        portfolioTreeModelFactory,
        logosStorage,
        portfolioTreeRecordFactory,
        instrumentWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialogFactory,
        settingsEditor,
        parent
    );
}
