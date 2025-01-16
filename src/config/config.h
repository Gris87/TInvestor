#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

#include <QMutex>



class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(QObject *parent = nullptr);
    explicit Config(const Config &config, QObject *parent = nullptr);
    ~Config();

    Config& operator=(const Config &config);

    void makeDefault();

    void save();
    void load();

    void setAutorun(bool value);
    bool isAutorun();

    void setRefreshTimeout(int value);
    int getRefreshTimeout();

    void setUseSchedule(bool value);
    bool isUseSchedule();

    void setScheduleStartHour(int value);
    int getScheduleStartHour();

    void setScheduleStartMinute(int value);
    int getScheduleStartMinute();

    void setScheduleEndHour(int value);
    int getScheduleEndHour();

    void setScheduleEndMinute(int value);
    int getScheduleEndMinute();

    void setLimitOperationsPerDay(bool value);
    bool isLimitOperationsPerDay();

    void setAmountOfOperationsPerDay(int value);
    int getAmountOfOperationsPerDay();

    void setLimitOperationsPerStock(bool value);
    bool isLimitOperationsPerStock();

    void setAmountOfOperationsPerStock(int value);
    int getAmountOfOperationsPerStock();

    void setUseForSimulatorAndAutoPilot(bool value);
    bool isUseForSimulatorAndAutoPilot();

private:
    void assign(const Config &config);

    QMutex mMutex;
    bool   mAutorun;
    int    mRefreshTimeout;
    bool   mUseSchedule;
    int    mScheduleStartHour;
    int    mScheduleStartMinute;
    int    mScheduleEndHour;
    int    mScheduleEndMinute;
    bool   mLimitOperationsPerDay;
    int    mAmountOfOperationsPerDay;
    bool   mLimitOperationsPerStock;
    int    mAmountOfOperationsPerStock;
    bool   mUseForSimulatorAndAutoPilot;
};



#endif // CONFIG_H
