#pragma once



#include "src/config/settingseditor/isettingseditor.h"



class ISellDecision1Config
{
public:
    ISellDecision1Config() {}
    virtual ~ISellDecision1Config() = default;

    ISellDecision1Config(const ISellDecision1Config &another) = delete;
    ISellDecision1Config& operator=(const ISellDecision1Config &another) = delete;

    virtual void assign(ISellDecision1Config *another) = 0;
    virtual void makeDefault() = 0;

    virtual void save(ISettingsEditor *settingsEditor, const QString &type) = 0;
    virtual void load(ISettingsEditor *settingsEditor, const QString &type) = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled() = 0;

    virtual void setIncomeAbove(float value) = 0;
    virtual float getIncomeAbove() = 0;
};
