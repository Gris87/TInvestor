#pragma once



#include "src/decisions/idecisionmaker.h"

#include "src/config/iconfig.h"
#include "src/storage/user/iuserstorage.h"



class DecisionMaker : public IDecisionMaker
{
public:
    DecisionMaker(IConfig* config, IUserStorage* userStorage);
    ~DecisionMaker() override;

    DecisionMaker(const DecisionMaker& another)            = delete;
    DecisionMaker& operator=(const DecisionMaker& another) = delete;

    InstrumentsForTrading makeDecision(const Portfolio& portfolio, const QList<Stock*>& stocks) override;

private:
    IConfig*      mConfig;
    IUserStorage* mUserStorage;
};
