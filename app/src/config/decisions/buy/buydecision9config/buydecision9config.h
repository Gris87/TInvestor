#pragma once



#include "src/config/decisions/buy/buydecision9config/ibuydecision9config.h"

#include <QReadWriteLock>



class BuyDecision9Config : public IBuyDecision9Config
{
public:
    BuyDecision9Config();
    ~BuyDecision9Config() override;

    BuyDecision9Config(const BuyDecision9Config& another)            = delete;
    BuyDecision9Config& operator=(const BuyDecision9Config& another) = delete;

    IBuyDecision9Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision9Config* another) override;
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

    void  setRsi(float value) override;
    float getRsi() override;

    void setDuration(int value) override;
    int  getDuration() override;

private:
    QReadWriteLock* mRwMutex;
    bool            mEnabled;
    float           mRsi;
    int             mDuration;
};
