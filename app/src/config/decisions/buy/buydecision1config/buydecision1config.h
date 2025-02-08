#pragma once



#include "src/config/decisions/buy/buydecision1config/ibuydecision1config.h"

#include <QMutex>



class BuyDecision1Config : public IBuyDecision1Config
{
public:
    BuyDecision1Config();
    ~BuyDecision1Config();

    BuyDecision1Config(const BuyDecision1Config& another)            = delete;
    BuyDecision1Config& operator=(const BuyDecision1Config& another) = delete;

    void assign(IBuyDecision1Config* another) override;
    void makeDefault() override;

    void save(ISettingsEditor* settingsEditor, const QString& type) override;
    void load(ISettingsEditor* settingsEditor, const QString& type) override;

    void setEnabled(bool value) override;
    bool isEnabled() override;

    void  setPriceFall(float value) override;
    float getPriceFall() override;

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QMutex* mMutex;
    bool    mEnabled;
    float   mPriceFall;
    int     mDuration;
};
