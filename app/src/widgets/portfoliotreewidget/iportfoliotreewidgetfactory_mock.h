#pragma once



#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory.h"

#include <gmock/gmock.h>



class PortfolioTreeWidgetFactoryMock : public IPortfolioTreeWidgetFactory
{
public:
    PortfolioTreeWidgetFactoryMock() :
        IPortfolioTreeWidgetFactory()
    {
    }
    ~PortfolioTreeWidgetFactoryMock() override = default;

    PortfolioTreeWidgetFactoryMock(const PortfolioTreeWidgetFactoryMock& another)            = delete;
    PortfolioTreeWidgetFactoryMock& operator=(const PortfolioTreeWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        IPortfolioTreeWidget*,
        newInstance,
        (IPortfolioTreeModelFactory * portfolioTreeModelFactory,
         ILogosStorage*      logosStorage,
         IFileDialogFactory* fileDialogFactory,
         ISettingsEditor*    settingsEditor,
         QWidget*            parent),
        (const, override)
    );
};
