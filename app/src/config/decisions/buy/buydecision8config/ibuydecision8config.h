#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IBuyDecision8Config
{
public:
    IBuyDecision8Config()          = default;
    virtual ~IBuyDecision8Config() = default;

    IBuyDecision8Config(const IBuyDecision8Config& another)            = delete;
    IBuyDecision8Config& operator=(const IBuyDecision8Config& another) = delete;

    virtual IBuyDecision8Config* clone()                              = 0;
    virtual void                 deleteRecursively()                  = 0;
    virtual void                 assign(IBuyDecision8Config* another) = 0;
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

    virtual void setDuration(int value) = 0;
    virtual int  getDuration()          = 0;
};
