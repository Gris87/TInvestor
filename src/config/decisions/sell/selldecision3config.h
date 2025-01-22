#ifndef SELLDECISION3CONFIG_H
#define SELLDECISION3CONFIG_H

#include <QMutex>



class SellDecision3Config
{
public:
    SellDecision3Config();
    ~SellDecision3Config();

    SellDecision3Config& operator=(const SellDecision3Config &config);

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

    void setEnabled(bool value);
    bool isEnabled();

    void setIncomeAbove(float value);
    float getIncomeAbove();

private:
    void assign(const SellDecision3Config &config);

    QMutex mMutex;
    bool   mEnabled;
    float  mIncomeAbove;
};



#endif // SELLDECISION3CONFIG_H
