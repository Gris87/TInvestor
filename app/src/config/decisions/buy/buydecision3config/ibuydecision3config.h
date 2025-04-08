#pragma once



#include "src/utils/settingseditor/isettingseditor.h"



class IBuyDecision3Config
{
public:
    IBuyDecision3Config()          = default;
    virtual ~IBuyDecision3Config() = default;

    IBuyDecision3Config(const IBuyDecision3Config& another)            = delete;
    IBuyDecision3Config& operator=(const IBuyDecision3Config& another) = delete;

    virtual void assign(IBuyDecision3Config* another) = 0;
    virtual void makeDefault()                        = 0;

    virtual void save(ISettingsEditor* settingsEditor, const QString& type) = 0;
    virtual void load(ISettingsEditor* settingsEditor, const QString& type) = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled()            = 0;

    virtual void  setPriceRise(float value) = 0;
    virtual float getPriceRise()            = 0;

    virtual void setDuration(int value) = 0;
    virtual int  getDuration()          = 0;
};
