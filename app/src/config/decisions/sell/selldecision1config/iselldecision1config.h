#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class ISellDecision1Config
{
public:
    ISellDecision1Config()          = default;
    virtual ~ISellDecision1Config() = default;

    ISellDecision1Config(const ISellDecision1Config& another)            = delete;
    ISellDecision1Config& operator=(const ISellDecision1Config& another) = delete;

    virtual ISellDecision1Config* clone()                               = 0;
    virtual void                  deleteRecursively()                   = 0;
    virtual void                  assign(ISellDecision1Config* another) = 0;
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

    virtual void  setYieldAbove(float value) = 0;
    virtual float getYieldAbove()            = 0;
};
