#ifndef SELLDECISION1CONFIG_H
#define SELLDECISION1CONFIG_H

#include <QMutex>



class SellDecision1Config
{
public:
    SellDecision1Config();
    ~SellDecision1Config();

    SellDecision1Config& operator=(const SellDecision1Config &config);

    void makeDefault();

    void save(const QString &type);
    void load(const QString &type);

    void setEnabled(bool value);
    bool isEnabled();

private:
    void assign(const SellDecision1Config &config);

    QMutex mMutex;
    bool   mEnabled;
};



#endif // SELLDECISION1CONFIG_H
