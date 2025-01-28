#pragma once



#include "src/config/isettingseditor.h"



class ISellDecision2Config
{
public:
    ISellDecision2Config() {}
    virtual ~ISellDecision2Config() = default;

    ISellDecision2Config(const ISellDecision2Config &another) = delete;
    ISellDecision2Config& operator=(const ISellDecision2Config &another) = delete;

    virtual void assign(ISellDecision2Config *another) = 0;
    virtual void makeDefault() = 0;

    virtual void save(ISettingsEditor *settingsEditor, const QString &type) = 0;
    virtual void load(ISettingsEditor *settingsEditor, const QString &type) = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled() = 0;

    virtual void setIncomeAbove(float value) = 0;
    virtual float getIncomeAbove() = 0;

    virtual void setLoseIncome(float value) = 0;
    virtual float getLoseIncome() = 0;
};
