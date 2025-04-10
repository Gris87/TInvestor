#pragma once



#include "src/utils/fs/zip/qzipfile/iqzipfilefactory.h"

#include <gmock/gmock.h>



class QZipFileFactoryMock : public IQZipFileFactory
{
public:
    QZipFileFactoryMock() :
        IQZipFileFactory()
    {
    }
    ~QZipFileFactoryMock() override = default;

    QZipFileFactoryMock(const QZipFileFactoryMock& another)            = delete;
    QZipFileFactoryMock& operator=(const QZipFileFactoryMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<IQZipFile>, newInstance, (QuaZip * zip), (const, override));
};
