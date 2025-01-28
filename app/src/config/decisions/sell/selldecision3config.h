#pragma once



#include "src/config/decisions/sell/iselldecision3config.h"

#include <QMutex>



class SellDecision3Config : public ISellDecision3Config
{
public:
    SellDecision3Config();
    ~SellDecision3Config();

    SellDecision3Config(const SellDecision3Config &another) = delete;
    SellDecision3Config& operator=(const SellDecision3Config &another) = delete;

    void assign(ISellDecision3Config *another) override;
    void makeDefault() override;

    void save(ISettingsEditor *settingsEditor, const QString &type) override;
    void load(ISettingsEditor *settingsEditor, const QString &type) override;

    void setEnabled(bool value) override;
    bool isEnabled() override;

    void setLoseIncome(float value) override;
    float getLoseIncome() override;

    void setDuration(int value) override;
    int getDuration() override;

private:
    QMutex *mMutex;
    bool   mEnabled;
    float  mLoseIncome;
    int    mDuration;
};
