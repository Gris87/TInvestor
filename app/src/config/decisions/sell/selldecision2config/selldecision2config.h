#pragma once



#include "src/config/decisions/sell/selldecision2config/iselldecision2config.h"

#include <QMutex>



class SellDecision2Config : public ISellDecision2Config
{
public:
    SellDecision2Config();
    ~SellDecision2Config() override;

    SellDecision2Config(const SellDecision2Config& another)            = delete;
    SellDecision2Config& operator=(const SellDecision2Config& another) = delete;

    ISellDecision2Config* clone() override;
    void                  deleteRecursively() override;
    void                  assign(ISellDecision2Config* another) override;
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

    void  setYieldAbove(float value) override;
    float getYieldAbove() override;

    void  setLoseYield(float value) override;
    float getLoseYield() override;

private:
    QMutex* mMutex;
    bool    mEnabled;
    float   mYieldAbove;
    float   mLoseYield;
};
