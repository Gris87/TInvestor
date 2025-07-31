#pragma once



#include "src/config/decisions/sell/selldecision4config/iselldecision4config.h"

#include <QMutex>



class SellDecision4Config : public ISellDecision4Config
{
public:
    SellDecision4Config();
    ~SellDecision4Config() override;

    SellDecision4Config(const SellDecision4Config& another)            = delete;
    SellDecision4Config& operator=(const SellDecision4Config& another) = delete;

    void assign(ISellDecision4Config* another) override;
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

private:
    QMutex* mMutex;
    bool    mEnabled;
    float   mLoseYield;
};
