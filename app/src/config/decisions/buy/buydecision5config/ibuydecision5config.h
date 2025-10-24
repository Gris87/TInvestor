#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IBuyDecision5Config
{
public:
    IBuyDecision5Config()          = default;
    virtual ~IBuyDecision5Config() = default;

    IBuyDecision5Config(const IBuyDecision5Config& another)            = delete;
    IBuyDecision5Config& operator=(const IBuyDecision5Config& another) = delete;

    virtual IBuyDecision5Config* clone()                              = 0;
    virtual void                 deleteRecursively()                  = 0;
    virtual void                 assign(IBuyDecision5Config* another) = 0;
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
