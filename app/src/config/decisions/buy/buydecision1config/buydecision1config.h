#pragma once



#include "src/config/decisions/buy/buydecision1config/ibuydecision1config.h"

#include <QReadWriteLock>



class BuyDecision1Config : public IBuyDecision1Config
{
public:
    BuyDecision1Config();
    ~BuyDecision1Config() override;

    BuyDecision1Config(const BuyDecision1Config& another)            = delete;
    BuyDecision1Config& operator=(const BuyDecision1Config& another) = delete;

    IBuyDecision1Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision1Config* another) override;
    void                 makeDefault(int commissionInt) override;

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

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    float           mPriceFall;
    int             mDuration;
};
