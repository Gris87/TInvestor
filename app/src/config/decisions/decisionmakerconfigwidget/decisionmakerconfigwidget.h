#pragma once



#include "src/config/decisions/idecisionmakerconfigwidget.h"

#include "src/config/decisions/buy/ibuydecision1configwidgetfactory.h"
#include "src/config/decisions/buy/ibuydecision2configwidgetfactory.h"
#include "src/config/decisions/buy/ibuydecision3configwidgetfactory.h"
#include "src/config/decisions/sell/iselldecision1configwidgetfactory.h"
#include "src/config/decisions/sell/iselldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/iselldecision3configwidgetfactory.h"
#include "src/config/decisions/idecisionmakerconfig.h"



namespace Ui {
class DecisionMakerConfigWidget;
}



class DecisionMakerConfigWidget : public IDecisionMakerConfigWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerConfigWidget(
        IDecisionMakerConfig *config,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        QWidget *parent = nullptr
    );
    ~DecisionMakerConfigWidget();

    DecisionMakerConfigWidget(const DecisionMakerConfigWidget &another) = delete;
    DecisionMakerConfigWidget& operator=(const DecisionMakerConfigWidget &another) = delete;

    void updateUiFromConfig() override;

    Ui::DecisionMakerConfigWidget *ui;

private:
    IBuyDecision1ConfigWidget  *mBuyDecision1ConfigWidget;
    IBuyDecision2ConfigWidget  *mBuyDecision2ConfigWidget;
    IBuyDecision3ConfigWidget  *mBuyDecision3ConfigWidget;
    ISellDecision1ConfigWidget *mSellDecision1ConfigWidget;
    ISellDecision2ConfigWidget *mSellDecision2ConfigWidget;
    ISellDecision3ConfigWidget *mSellDecision3ConfigWidget;
};
