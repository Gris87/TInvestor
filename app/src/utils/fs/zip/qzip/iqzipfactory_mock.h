#pragma once



#include "src/utils/fs/zip/qzip/iqzipfactory.h"

#include <gmock/gmock.h>



class QZipFactoryMock : public IQZipFactory
{
public:
    QZipFactoryMock() :
        IQZipFactory()
    {
    }
    ~QZipFactoryMock() override = default;

    QZipFactoryMock(const QZipFactoryMock& another)            = delete;
    QZipFactoryMock& operator=(const QZipFactoryMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<IQZip>, newInstance, (const QString& name), (const, override));
};
