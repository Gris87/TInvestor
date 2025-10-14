#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IBuyDecision9Config
{
public:
    IBuyDecision9Config()          = default;
    virtual ~IBuyDecision9Config() = default;

    IBuyDecision9Config(const IBuyDecision9Config& another)            = delete;
    IBuyDecision9Config& operator=(const IBuyDecision9Config& another) = delete;

    virtual IBuyDecision9Config* clone()                              = 0;
    virtual void                 deleteRecursively()                  = 0;
    virtual void                 assign(IBuyDecision9Config* another) = 0;
    virtual void                 makeDefault()                        = 0;

    virtual void save(ISettingsEditor* settingsEditor, const QString& type) = 0;
    virtual void load(ISettingsEditor* settingsEditor, const QString& type) = 0;

    virtual void fromJsonObject(simdjson::ondemand::object jsonObject) = 0; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    virtual QString toJsonString() const = 0;

    [[nodiscard]]
    virtual QStringList variantsAsJson() const = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled()            = 0;

    virtual void  setRsi(float value) = 0;
    virtual float getRsi()            = 0;
};
