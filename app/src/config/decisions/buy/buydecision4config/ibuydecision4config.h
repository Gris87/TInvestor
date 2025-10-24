#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IBuyDecision4Config
{
public:
    IBuyDecision4Config()          = default;
    virtual ~IBuyDecision4Config() = default;

    IBuyDecision4Config(const IBuyDecision4Config& another)            = delete;
    IBuyDecision4Config& operator=(const IBuyDecision4Config& another) = delete;

    virtual IBuyDecision4Config* clone()                              = 0;
    virtual void                 deleteRecursively()                  = 0;
    virtual void                 assign(IBuyDecision4Config* another) = 0;
    virtual void                 makeDefault(int commissionInt)       = 0;

    virtual void save(ISettingsEditor* settingsEditor, const QString& type) = 0;
    virtual void load(ISettingsEditor* settingsEditor, const QString& type) = 0;

    virtual void fromJsonObject(simdjson::ondemand::object jsonObject) = 0; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    virtual QString toJsonString() const = 0;

    [[nodiscard]]
    virtual QStringList variantsAsJson() const = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled()            = 0;

    virtual void  setPriceFall(float value) = 0;
    virtual float getPriceFall()            = 0;

    virtual void  setLoseYield(float value) = 0;
    virtual float getLoseYield()            = 0;

    virtual void setDuration(int value) = 0;
    virtual int  getDuration()          = 0;
};
