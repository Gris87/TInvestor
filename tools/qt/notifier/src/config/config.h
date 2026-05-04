#pragma once



#include "src/config/iconfig.h"

#include <QReadWriteLock>



class Config : public IConfig
{
public:
    explicit Config();
    ~Config() override;

    Config(const Config& another)            = delete;
    Config& operator=(const Config& another) = delete;

    void assign(IConfig* another) override;
    void makeDefault() override;

    void save(ISettingsEditor* settingsEditor) override;
    void load(ISettingsEditor* settingsEditor) override;

    void setAutorun(bool value) override;
    bool isAutorun() override;

    void    setServerAddress(const QString& value) override;
    QString getServerAddress() override;

    void setServerPort(int value) override;
    int  getServerPort() override;

    void setNotificationsEnabled(bool value) override;
    bool isNotificationsEnabled() override;

    void setFilterSystem(bool value) override;
    bool isFilterSystem() override;

    void setFilterPortfolio(bool value) override;
    bool isFilterPortfolio() override;

    void setFilterHugeSell(bool value) override;
    bool isFilterHugeSell() override;

    void setFilterDividends(bool value) override;
    bool isFilterDividends() override;

    void setFilterPulse(bool value) override;
    bool isFilterPulse() override;

private:
    QReadWriteLock* mRwMutex;

    bool    mAutorun;
    QString mServerAddress;
    int     mServerPort;
    bool    mNotificationsEnabled;
    bool    mFilterSystem;
    bool    mFilterPortfolio;
    bool    mFilterHugeSell;
    bool    mFilterDividends;
    bool    mFilterPulse;
};
