#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class ISellDecision2Config
{
public:
    ISellDecision2Config()          = default;
    virtual ~ISellDecision2Config() = default;

    ISellDecision2Config(const ISellDecision2Config& another)            = delete;
    ISellDecision2Config& operator=(const ISellDecision2Config& another) = delete;

    virtual void assign(ISellDecision2Config* another) = 0;
    virtual void makeDefault()                         = 0;

    virtual void save(ISettingsEditor* settingsEditor, const QString& type) = 0;
    virtual void load(ISettingsEditor* settingsEditor, const QString& type) = 0;

    virtual void fromJsonObject(simdjson::ondemand::object jsonObject) = 0; // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    virtual QString toJsonString() const = 0;

    [[nodiscard]]
    virtual QStringList variantsAsJson() const = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled()            = 0;

    virtual void  setYieldAbove(float value) = 0;
    virtual float getYieldAbove()            = 0;

    virtual void  setLoseYield(float value) = 0;
    virtual float getLoseYield()            = 0;
};
