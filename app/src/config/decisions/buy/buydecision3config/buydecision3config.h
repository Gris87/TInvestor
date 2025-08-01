#pragma once



#include "src/config/decisions/buy/buydecision3config/ibuydecision3config.h"

#include <QMutex>



class BuyDecision3Config : public IBuyDecision3Config
{
public:
    BuyDecision3Config();
    ~BuyDecision3Config() override;

    BuyDecision3Config(const BuyDecision3Config& another)            = delete;
    BuyDecision3Config& operator=(const BuyDecision3Config& another) = delete;

    void assign(IBuyDecision3Config* another) override;
    void makeDefault() override;

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
    QMutex* mMutex;
    bool    mEnabled;
    float   mPriceFall;
    int     mDuration;
};
