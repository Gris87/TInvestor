#pragma once



#include "src/config/decisions/buy/buydecision5config/ibuydecision5config.h"

#include <QReadWriteLock>



class BuyDecision5Config : public IBuyDecision5Config
{
public:
    BuyDecision5Config();
    ~BuyDecision5Config() override;

    BuyDecision5Config(const BuyDecision5Config& another)            = delete;
    BuyDecision5Config& operator=(const BuyDecision5Config& another) = delete;

    IBuyDecision5Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision5Config* another) override;
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

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    int             mDuration;
};
