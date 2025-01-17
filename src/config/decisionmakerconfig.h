#ifndef DECISIONMAKERCONFIG_H
#define DECISIONMAKERCONFIG_H

#include <QMutex>



class DecisionMakerConfig
{
public:
    DecisionMakerConfig();
    ~DecisionMakerConfig();

    DecisionMakerConfig& operator=(const DecisionMakerConfig &config);

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

    void setAutorun(bool value);
    bool isAutorun();

private:
    void assign(const DecisionMakerConfig &config);

    QMutex mMutex;
    bool   mAutorun;
};



#endif // DECISIONMAKERCONFIG_H
