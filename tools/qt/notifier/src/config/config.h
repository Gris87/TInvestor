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

    void     assign(IConfig* another) override;
    void     makeDefault() override;

    void save(ISettingsEditor* settingsEditor) override;
    void load(ISettingsEditor* settingsEditor) override;

    void setAutorun(bool value) override;
    bool isAutorun() override;

    void    setServerAddress(const QString& value) override;
    QString getServerAddress() override;

    void setServerPort(int value) override;
    int  getServerPort() override;

private:
    QReadWriteLock* mRwMutex;

    bool    mAutorun;
    QString mServerAddress;
    int     mServerPort;
};
