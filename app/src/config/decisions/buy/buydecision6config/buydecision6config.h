#pragma once



#include "src/config/decisions/buy/buydecision6config/ibuydecision6config.h"

#include <QReadWriteLock>



class BuyDecision6Config : public IBuyDecision6Config
{
public:
    BuyDecision6Config();
    ~BuyDecision6Config() override;

    BuyDecision6Config(const BuyDecision6Config& another)            = delete;
    BuyDecision6Config& operator=(const BuyDecision6Config& another) = delete;

    IBuyDecision6Config* clone() override;
    void                 deleteRecursively() override;
    void                 assign(IBuyDecision6Config* another) override;
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
