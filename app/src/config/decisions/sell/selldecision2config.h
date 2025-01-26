#pragma once



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

    void setLoseIncome(float value);
    float getLoseIncome();

private:
    void assign(const SellDecision2Config &config);

    QMutex *mMutex;
    bool   mEnabled;
    float  mIncomeAbove;
    float  mLoseIncome;
};
