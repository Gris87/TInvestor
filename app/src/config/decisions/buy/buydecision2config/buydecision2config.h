#pragma once



#include "src/config/decisions/buy/buydecision2config/ibuydecision2config.h"

#include <QMutex>



class BuyDecision2Config : public IBuyDecision2Config
{
public:
    BuyDecision2Config();
    ~BuyDecision2Config() override;

    BuyDecision2Config(const BuyDecision2Config& another)            = delete;
    BuyDecision2Config& operator=(const BuyDecision2Config& another) = delete;

    IBuyDecision2Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision2Config* another) override;
    void                 makeDefault() override;

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
    QMutex* mMutex;
    bool    mEnabled;
    float   mPriceFall;
    float   mLoseYield;
    int     mDuration;
};
