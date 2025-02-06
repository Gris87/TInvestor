#pragma once



#include "src/config/settingseditor/isettingseditor.h"



class ISellDecision3Config
{
public:
    ISellDecision3Config() {}
    virtual ~ISellDecision3Config() = default;

    ISellDecision3Config(const ISellDecision3Config &another) = delete;
    ISellDecision3Config& operator=(const ISellDecision3Config &another) = delete;

    virtual void assign(ISellDecision3Config *another) = 0;
    virtual void makeDefault() = 0;

    virtual void save(ISettingsEditor *settingsEditor, const QString &type) = 0;
    virtual void load(ISettingsEditor *settingsEditor, const QString &type) = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled() = 0;

    virtual void setLoseIncome(float value) = 0;
    virtual float getLoseIncome() = 0;

    virtual void setDuration(int value) = 0;
    virtual int getDuration() = 0;
};
