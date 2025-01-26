#ifndef IDECISIONMAKERCONFIG_H
#define IDECISIONMAKERCONFIG_H

#include <QString>



class IDecisionMakerConfig
{
public:
    virtual void makeDefault() = 0;
    virtual void save(const QString &type) = 0;
    virtual void load(const QString &type) = 0;
};



#endif // IDECISIONMAKERCONFIG_H
