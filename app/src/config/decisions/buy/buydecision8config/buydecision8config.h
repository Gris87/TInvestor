#pragma once



#include "src/config/decisions/buy/buydecision8config/ibuydecision8config.h"

#include <QReadWriteLock>



class BuyDecision8Config : public IBuyDecision8Config
{
public:
    BuyDecision8Config();
    ~BuyDecision8Config() override;

    BuyDecision8Config(const BuyDecision8Config& another)            = delete;
    BuyDecision8Config& operator=(const BuyDecision8Config& another) = delete;

    IBuyDecision8Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision8Config* another) override;
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

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    int             mDuration;
};
