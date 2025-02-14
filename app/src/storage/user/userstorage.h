#pragma once



#include "src/storage/user/iuserstorage.h"



class UserStorage : public IUserStorage
{
public:
    explicit UserStorage();
    ~UserStorage();

    UserStorage(const UserStorage& another)            = delete;
    UserStorage& operator=(const UserStorage& another) = delete;

    void           readFromDatabase(IUserDatabase* userDatabase) override;
    const QString& getToken() override;

private:
    User mUser;
};
