#pragma once



#include "src/config/decisions/buy/buydecision7config/ibuydecision7config.h"

#include <QReadWriteLock>



class BuyDecision7Config : public IBuyDecision7Config
{
public:
    BuyDecision7Config();
    ~BuyDecision7Config() override;

    BuyDecision7Config(const BuyDecision7Config& another)            = delete;
    BuyDecision7Config& operator=(const BuyDecision7Config& another) = delete;

    IBuyDecision7Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision7Config* another) override;
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

    void  setPriceRaise(float value) override;
    float getPriceRaise() override;

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    float           mPriceRaise;
    int             mDuration;
};
