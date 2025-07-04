#pragma once



#include "src/db/logos/ilogosdatabase.h"

#include <gmock/gmock.h>



class LogosDatabaseMock : public ILogosDatabase
{
public:
    explicit LogosDatabaseMock() :
        ILogosDatabase()
    {
    }
    ~LogosDatabaseMock() override = default;

    LogosDatabaseMock(const LogosDatabaseMock& another)            = delete;
    LogosDatabaseMock& operator=(const LogosDatabaseMock& another) = delete;

    MOCK_METHOD(Logos, prepareLogos, (), (override));
    MOCK_METHOD(void, readLogo, (const QString& instrumentId, QPixmap* logo), (override));
    MOCK_METHOD(void, writeLogo, (const QString& instrumentId, QPixmap* logo), (override));
};
