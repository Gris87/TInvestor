#pragma once



#include <QThread>



class IUserUpdateThread : public QThread
{
    Q_OBJECT

public:
    explicit IUserUpdateThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IUserUpdateThread() override = default;

    IUserUpdateThread(const IUserUpdateThread& another)            = delete;
    IUserUpdateThread& operator=(const IUserUpdateThread& another) = delete;
};
