#pragma once



#include <QString>



class IBuyDecision2Config
{
public:
    IBuyDecision2Config() {}
    virtual ~IBuyDecision2Config() = default;

    IBuyDecision2Config(const IBuyDecision2Config &another) = delete;
    IBuyDecision2Config& operator=(const IBuyDecision2Config &another) = delete;

    virtual void assign(IBuyDecision2Config *another) = 0;
    virtual void makeDefault() = 0;

    virtual void save(const QString &type) = 0;
    virtual void load(const QString &type) = 0;

    virtual void setEnabled(bool value) = 0;
    virtual bool isEnabled() = 0;

    virtual void setPriceDiff(float value) = 0;
    virtual float getPriceDiff() = 0;

    virtual void setAmountOfTimes(int value) = 0;
    virtual int getAmountOfTimes() = 0;

    virtual void setDuration(int value) = 0;
    virtual int getDuration() = 0;
};
