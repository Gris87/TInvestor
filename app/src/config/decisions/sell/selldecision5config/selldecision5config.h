#pragma once



#include "src/config/decisions/sell/selldecision5config/iselldecision5config.h"

#include <QReadWriteLock>



class SellDecision5Config : public ISellDecision5Config
{
public:
    SellDecision5Config();
    ~SellDecision5Config() override;

    SellDecision5Config(const SellDecision5Config& another)            = delete;
    SellDecision5Config& operator=(const SellDecision5Config& another) = delete;

    ISellDecision5Config* clone() override;
    void                  deleteRecursively() override;
    void                  assign(ISellDecision5Config* another) override;
    void                  makeDefault() override;

    void save(ISettingsEditor* settingsEditor, const QString& type) override;
    void load(ISettingsEditor* settingsEditor, const QString& type) override;

    void fromJsonObject(simdjson::ondemand::object jsonObject) override; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QString toJsonString() const override;

    [[nodiscard]]
    QStringList variantsAsJson() const override;

    void setEnabled(bool value) override;
    bool isEnabled() override;

    void  setLoseYield(float value) override;
    float getLoseYield() override;

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    float           mLoseYield;
    int             mDuration;
};
