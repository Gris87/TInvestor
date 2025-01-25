#ifndef BUYDECISION3CONFIG_H
#define BUYDECISION3CONFIG_H

#include <QMutex>



class BuyDecision3Config
{
public:
    BuyDecision3Config();
    ~BuyDecision3Config();

    BuyDecision3Config& operator=(const BuyDecision3Config &config);

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

    void setEnabled(bool value);
    bool isEnabled();

    void setPriceRise(float value);
    float getPriceRise();

    void setDuration(int value);
    int getDuration();

private:
    void assign(const BuyDecision3Config &config);

    QMutex *mMutex;
    bool   mEnabled;
    float  mPriceRise;
    int    mDuration;
};



#endif // BUYDECISION3CONFIG_H
