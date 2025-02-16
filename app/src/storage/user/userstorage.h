#pragma once



#include "src/storage/user/iuserstorage.h"

#include "src/db/user/iuserdatabase.h"



class UserStorage : public IUserStorage
{
public:
    explicit UserStorage(IUserDatabase* userDatabase);
    ~UserStorage();

    UserStorage(const UserStorage& another)            = delete;
    UserStorage& operator=(const UserStorage& another) = delete;

    void           readFromDatabase() override;
    const QString& getToken() override;
    void           setToken(const QString& token) override;

private:
    IUserDatabase* mUserDatabase;
    User           mUser;
};
