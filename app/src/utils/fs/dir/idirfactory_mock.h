#pragma once



#include "src/utils/fs/dir/idirfactory.h"

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

    MOCK_METHOD(std::shared_ptr<IDir>, newInstance, (const QString& path), (override));
};
