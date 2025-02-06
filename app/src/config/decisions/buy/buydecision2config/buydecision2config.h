#pragma once



#include "src/config/decisions/buy/buydecision2config/ibuydecision2config.h"

#include <QMutex>



class BuyDecision2Config : public IBuyDecision2Config
{
public:
    BuyDecision2Config();
    ~BuyDecision2Config();

    BuyDecision2Config(const BuyDecision2Config &another) = delete;
    BuyDecision2Config& operator=(const BuyDecision2Config &another) = delete;

    void assign(IBuyDecision2Config *another) override;
    void makeDefault() override;

    void save(ISettingsEditor *settingsEditor, const QString &type) override;
    void load(ISettingsEditor *settingsEditor, const QString &type) override;

    void setEnabled(bool value) override;
    bool isEnabled() override;

    void setPriceDiff(float value) override;
    float getPriceDiff() override;

    void setAmountOfTimes(int value) override;
    int getAmountOfTimes() override;

    void setDuration(int value) override;
    int getDuration() override;

private:
    QMutex *mMutex;
    bool   mEnabled;
    float  mPriceDiff;
    int    mAmountOfTimes;
    int    mDuration;
};
