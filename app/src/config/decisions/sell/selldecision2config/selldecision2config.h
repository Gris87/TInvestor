#pragma once



#include "src/config/decisions/sell/selldecision2config/iselldecision2config.h"

#include <QMutex>



class SellDecision2Config : public ISellDecision2Config
{
public:
    SellDecision2Config();
    ~SellDecision2Config();

    SellDecision2Config(const SellDecision2Config &another) = delete;
    SellDecision2Config& operator=(const SellDecision2Config &another) = delete;

    void assign(ISellDecision2Config *another) override;
    void makeDefault() override;

    void save(ISettingsEditor *settingsEditor, const QString &type) override;
    void load(ISettingsEditor *settingsEditor, const QString &type) override;

    void setEnabled(bool value) override;
    bool isEnabled() override;

    void setIncomeAbove(float value) override;
    float getIncomeAbove() override;

    void setLoseIncome(float value) override;
    float getLoseIncome() override;

private:
    QMutex *mMutex;
    bool   mEnabled;
    float  mIncomeAbove;
    float  mLoseIncome;
};
