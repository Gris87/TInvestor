#ifndef BUYDECISION1CONFIG_H
#define BUYDECISION1CONFIG_H

#include <QMutex>



class BuyDecision1Config
{
public:
    BuyDecision1Config();
    ~BuyDecision1Config();

    BuyDecision1Config& operator=(const BuyDecision1Config &config);

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

    void setEnabled(bool value);
    bool isEnabled();

    void setPriceFall(float value);
    float getPriceFall();

    void setDuration(int value);
    int getDuration();

private:
    void assign(const BuyDecision1Config &config);

    QMutex mMutex;
    bool   mEnabled;
    float  mPriceFall;
    int    mDuration;
};



#endif // BUYDECISION1CONFIG_H
