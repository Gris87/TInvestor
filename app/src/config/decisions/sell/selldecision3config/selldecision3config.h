#pragma once



#include "src/config/decisions/sell/selldecision3config/iselldecision3config.h"

#include <QMutex>



class SellDecision3Config : public ISellDecision3Config
{
public:
    SellDecision3Config();
    ~SellDecision3Config() override;

    SellDecision3Config(const SellDecision3Config& another)            = delete;
    SellDecision3Config& operator=(const SellDecision3Config& another) = delete;

    void assign(ISellDecision3Config* another) override;
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

    void  setLoseYield(float value) override;
    float getLoseYield() override;

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QMutex* mMutex;
    bool    mEnabled;
    float   mLoseYield;
    int     mDuration;
};
