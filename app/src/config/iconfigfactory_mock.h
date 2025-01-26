#pragma once



#include <gmock/gmock.h>

#include "src/config/decisions/iconfigfactory.h"



class ConfigFactoryMock : public IConfigFactory
{
public:
    ~ConfigFactoryMock() override = default;

    MOCK_METHOD0(newInstance, IConfig*());
};
