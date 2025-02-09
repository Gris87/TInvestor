#pragma once



#include "src/fs/dir/idirfactory.h"

#include <gmock/gmock.h>



class DirFactoryMock : public IDirFactory
{
public:
    DirFactoryMock() :
        IDirFactory()
    {
    }
    ~DirFactoryMock() override = default;

    DirFactoryMock(const DirFactoryMock& another)            = delete;
    DirFactoryMock& operator=(const DirFactoryMock& another) = delete;

    MOCK_METHOD0(newInstance, IDir*());
};
