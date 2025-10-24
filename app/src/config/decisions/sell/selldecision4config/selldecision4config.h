#pragma once



#include "src/config/decisions/sell/selldecision4config/iselldecision4config.h"

#include <QReadWriteLock>



class SellDecision4Config : public ISellDecision4Config
{
public:
    SellDecision4Config();
    ~SellDecision4Config() override;

    SellDecision4Config(const SellDecision4Config& another)            = delete;
    SellDecision4Config& operator=(const SellDecision4Config& another) = delete;

    ISellDecision4Config* clone() override;
    void                  deleteRecursively() override;
    void                  assign(ISellDecision4Config* another) override;
    void                  makeDefault(int commissionInt) override;

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

    void  setYieldAbove(float value) override;
    float getYieldAbove() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    int             mDuration;
    float           mYieldAbove;
};
