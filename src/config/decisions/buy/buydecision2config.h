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

    void setPriceDiff(float value);
    float getPriceDiff();

    void setAmountOfTimes(int value);
    int getAmountOfTimes();

    void setDuration(int value);
    int getDuration();

private:
    void assign(const BuyDecision2Config &config);

    QMutex *mMutex;
    bool   mEnabled;
    float  mPriceDiff;
    int    mAmountOfTimes;
    int    mDuration;
};



#endif // BUYDECISION2CONFIG_H
