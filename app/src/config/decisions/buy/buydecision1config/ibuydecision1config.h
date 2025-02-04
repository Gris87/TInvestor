#pragma once



#include "src/config/settingseditor/isettingseditor.h"



class IBuyDecision1Config
{
public:
    IBuyDecision1Config() {}
    virtual ~IBuyDecision1Config() = default;

    IBuyDecision1Config(const IBuyDecision1Config &another) = delete;
    IBuyDecision1Config& operator=(const IBuyDecision1Config &another) = delete;

    virtual void assign(IBuyDecision1Config *another) = 0;
    virtual void makeDefault() = 0;

    virtual void save(ISettingsEditor *settingsEditor, const QString &type) = 0;
    virtual void load(ISettingsEditor *settingsEditor, const QString &type) = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled() = 0;

    virtual void setPriceFall(float value) = 0;
    virtual float getPriceFall() = 0;

    virtual void setDuration(int value) = 0;
    virtual int getDuration() = 0;
};
