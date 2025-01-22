#ifndef SELLDECISION2CONFIG_H
#define SELLDECISION2CONFIG_H

#include <QMutex>



class SellDecision2Config
{
public:
    SellDecision2Config();
    ~SellDecision2Config();

    SellDecision2Config& operator=(const SellDecision2Config &config);

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

    void setEnabled(bool value);
    bool isEnabled();

    void setIncomeAbove(float value);
    float getIncomeAbove();

private:
    void assign(const SellDecision2Config &config);

    QMutex mMutex;
    bool   mEnabled;
    float  mIncomeAbove;
};



#endif // SELLDECISION2CONFIG_H
