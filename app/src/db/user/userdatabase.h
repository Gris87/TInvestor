#pragma once



#include "src/db/user/iuserdatabase.h"

#include <QtSql>



class UserDatabase : public IUserDatabase
{
public:
    explicit UserDatabase();
    ~UserDatabase();

    UserDatabase(const UserDatabase& another)            = delete;
    UserDatabase& operator=(const UserDatabase& another) = delete;

    User readUserInfo() override;
    void writeToken(const QString& token) override;
    void writeUserInfo(const User& user) override;

    QSqlDatabase db;

private:
    void createUserTable();
};
