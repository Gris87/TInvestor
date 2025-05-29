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
    IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
    IInstrumentWidgetFactory*    instrumentWidgetFactory,
    IUserStorage*                userStorage,
    IInstrumentsStorage*         instrumentsStorage,
    ISettingsEditor*             settingsEditor,
    QWidget*                     parent
) const
{
    return new PortfolioTreeWidget(
        portfolioTreeRecordFactory, instrumentWidgetFactory, userStorage, instrumentsStorage, settingsEditor, parent
    );
}
