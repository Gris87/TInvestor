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
         IFileDialogFactory* fileDialogFactory,
         IMessageBoxUtils*   messageBoxUtils,
         ISettingsEditor*    settingsEditor,
         QWidget*            parent),
        (const, override)
    );
};
