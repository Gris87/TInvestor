#pragma once



#include "src/config/decisions/buy/buydecision4config/ibuydecision4config.h"

#include <QReadWriteLock>



class BuyDecision4Config : public IBuyDecision4Config
{
public:
    BuyDecision4Config();
    ~BuyDecision4Config() override;

    BuyDecision4Config(const BuyDecision4Config& another)            = delete;
    BuyDecision4Config& operator=(const BuyDecision4Config& another) = delete;

    IBuyDecision4Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision4Config* another) override;
    void                 makeDefault(int commissionInt) override;
    bool                 isDefault(int commissionInt) override;

    void save(ISettingsEditor* settingsEditor, const QString& type) override;
    void load(ISettingsEditor* settingsEditor, const QString& type) override;

    void fromJsonObject(simdjson::ondemand::object jsonObject) override; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QString toJsonString() const override;

    [[nodiscard]]
    QStringList variantsAsJson() const override;

    void setEnabled(bool value) override;
    bool isEnabled() override;

    void  setPriceFall(float value) override;
    float getPriceFall() override;

    void  setLoseYield(float value) override;
    float getLoseYield() override;

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    float           mPriceFall;
    float           mLoseYield;
    int             mDuration;
};
