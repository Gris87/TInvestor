#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget.h"

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidgetfactory.h"
#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidgetfactory.h"



namespace Ui
{
class DecisionMakerConfigWidget;
}



class DecisionMakerConfigWidget : public IDecisionMakerConfigWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerConfigWidget(
        IDecisionMakerConfig*              config,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory,
        IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory,
        IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory,
        IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory,
        IBuyDecision8ConfigWidgetFactory*  buyDecision8ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory,
        QWidget*                           parent = nullptr
    );
    ~DecisionMakerConfigWidget() override;

    DecisionMakerConfigWidget(const DecisionMakerConfigWidget& another)            = delete;
    DecisionMakerConfigWidget& operator=(const DecisionMakerConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::DecisionMakerConfigWidget* ui;

private:
    IBuyDecision1ConfigWidget*  mBuyDecision1ConfigWidget;
    IBuyDecision2ConfigWidget*  mBuyDecision2ConfigWidget;
    IBuyDecision3ConfigWidget*  mBuyDecision3ConfigWidget;
    IBuyDecision4ConfigWidget*  mBuyDecision4ConfigWidget;
    IBuyDecision5ConfigWidget*  mBuyDecision5ConfigWidget;
    IBuyDecision6ConfigWidget*  mBuyDecision6ConfigWidget;
    IBuyDecision7ConfigWidget*  mBuyDecision7ConfigWidget;
    IBuyDecision8ConfigWidget*  mBuyDecision8ConfigWidget;
    ISellDecision1ConfigWidget* mSellDecision1ConfigWidget;
    ISellDecision2ConfigWidget* mSellDecision2ConfigWidget;
    ISellDecision3ConfigWidget* mSellDecision3ConfigWidget;
    ISellDecision4ConfigWidget* mSellDecision4ConfigWidget;
};
