#pragma once



#include <QString>



class IDecisionMakerConfig
{
public:
    virtual ~IDecisionMakerConfig() = default;

    virtual void assign(IDecisionMakerConfig *another) = 0;
    virtual void makeDefault() = 0;
    virtual void save(const QString &type) = 0;
    virtual void load(const QString &type) = 0;
};
