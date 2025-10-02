#pragma once



#include <Qt>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IBuyDecision7Config
{
public:
    IBuyDecision7Config()          = default;
    virtual ~IBuyDecision7Config() = default;

    IBuyDecision7Config(const IBuyDecision7Config& another)            = delete;
    IBuyDecision7Config& operator=(const IBuyDecision7Config& another) = delete;

    virtual IBuyDecision7Config* clone()                              = 0;
    virtual void                 deleteRecursively()                  = 0;
    virtual void                 assign(IBuyDecision7Config* another) = 0;
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

    virtual void  setPriceRaise(float value) = 0;
    virtual float getPriceRaise()            = 0;

    virtual void setDuration(int value) = 0;
    virtual int  getDuration()          = 0;
};
