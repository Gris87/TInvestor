#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class ISellDecision5Config
{
public:
    ISellDecision5Config()          = default;
    virtual ~ISellDecision5Config() = default;

    ISellDecision5Config(const ISellDecision5Config& another)            = delete;
    ISellDecision5Config& operator=(const ISellDecision5Config& another) = delete;

    virtual ISellDecision5Config* clone()                               = 0;
    virtual void                  deleteRecursively()                   = 0;
    virtual void                  assign(ISellDecision5Config* another) = 0;
    virtual void                  makeDefault()                         = 0;

    virtual void save(ISettingsEditor* settingsEditor, const QString& type) = 0;
    virtual void load(ISettingsEditor* settingsEditor, const QString& type) = 0;

    virtual void fromJsonObject(simdjson::ondemand::object jsonObject) = 0; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    virtual QString toJsonString() const = 0;

    [[nodiscard]]
    virtual QStringList variantsAsJson() const = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled()            = 0;

    virtual void  setLoseYield(float value) = 0;
    virtual float getLoseYield()            = 0;

    virtual void setDuration(int value) = 0;
    virtual int  getDuration()          = 0;
};
