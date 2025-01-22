#ifndef BUYDECISION2CONFIG_H
#define BUYDECISION2CONFIG_H

#include <QMutex>



class BuyDecision2Config
{
public:
    BuyDecision2Config();
    ~BuyDecision2Config();

    BuyDecision2Config& operator=(const BuyDecision2Config &config);

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

    void setEnabled(bool value);
    bool isEnabled();

    void setIncomeAbove(float value);
    float getIncomeAbove();

private:
    void assign(const BuyDecision2Config &config);

    QMutex mMutex;
    bool   mEnabled;
    float  mIncomeAbove;
};



#endif // BUYDECISION2CONFIG_H
