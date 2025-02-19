#pragma once



#include "src/threads/userupdate/iuserupdatethread.h"

#include "src/config/iconfig.h"
#include "src/storage/stocks/istocksstorage.h"



class UserUpdateThread : public IUserUpdateThread
{
    Q_OBJECT

public:
    explicit UserUpdateThread(
        IConfig* config, IStocksDatabase* stocksDatabase, IStocksStorage* stocksStorage, QObject* parent = nullptr
    );
    ~UserUpdateThread();

    UserUpdateThread(const UserUpdateThread& another)            = delete;
    UserUpdateThread& operator=(const UserUpdateThread& another) = delete;

    void run() override;

private:
    IConfig*         mConfig;
    IStocksDatabase* mStocksDatabase;
    IStocksStorage*  mStocksStorage;
};
