#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>



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

private:
    void assign(const Config &config);

    bool mUseForSimulatorAndAutoPilot;
};



#endif // CONFIG_H
