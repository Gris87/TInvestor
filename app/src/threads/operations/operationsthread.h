#pragma once



#include "src/threads/operations/ioperationsthread.h"

#include "src/storage/user/iuserstorage.h"



class OperationsThread : public IOperationsThread
{
    Q_OBJECT

public:
    explicit OperationsThread(IUserStorage* userStorage, QObject* parent = nullptr);
    ~OperationsThread() override;

    OperationsThread(const OperationsThread& another)            = delete;
    OperationsThread& operator=(const OperationsThread& another) = delete;

    void run() override;

private:
    IUserStorage* mUserStorage;
};
