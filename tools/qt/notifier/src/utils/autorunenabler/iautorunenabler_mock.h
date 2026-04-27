#pragma once



#include "src/utils/autorunenabler/iautorunenabler.h"

#include <gmock/gmock.h>



class AutorunEnablerMock : public IAutorunEnabler
{
public:
    AutorunEnablerMock() :
        IAutorunEnabler()
    {
    }
    ~AutorunEnablerMock() override = default;

    AutorunEnablerMock(const AutorunEnablerMock& another)            = delete;
    AutorunEnablerMock& operator=(const AutorunEnablerMock& another) = delete;

    MOCK_METHOD(void, setEnabled, (bool enabled), (override));
};
