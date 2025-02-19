#pragma once



#include "src/threads/userupdate/iuserupdatethread.h"



class UserUpdateThread : public IUserUpdateThread
{
    Q_OBJECT

public:
    explicit UserUpdateThread(QObject* parent = nullptr);
    ~UserUpdateThread();

    UserUpdateThread(const UserUpdateThread& another)            = delete;
    UserUpdateThread& operator=(const UserUpdateThread& another) = delete;

    void run() override;
};
