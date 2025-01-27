#pragma once



#include "src/config/decisions/sell/iselldecision1config.h"

#include <QMutex>



class SellDecision1Config : public ISellDecision1Config
{
public:
    SellDecision1Config();
    ~SellDecision1Config();

    SellDecision1Config(const SellDecision1Config &another) = delete;
    SellDecision1Config& operator=(const SellDecision1Config &another) = delete;

    void assign(ISellDecision1Config *another) override;
    void makeDefault() override;

    void save(const QString &type) override;
    void load(const QString &type) override;

    void setEnabled(bool value) override;
    bool isEnabled() override;

    void setIncomeAbove(float value) override;
    float getIncomeAbove() override;

private:
    QMutex *mMutex;
    bool   mEnabled;
    float  mIncomeAbove;
};
