#pragma once



#include "src/utils/fs/zip/qzipfile/iqzipfile.h"

#include <gmock/gmock.h>



class QZipFileMock : public IQZipFile
{
public:
    QZipFileMock() :
        IQZipFile()
    {
    }
    ~QZipFileMock() override = default;

    QZipFileMock(const QZipFileMock& another)            = delete;
    QZipFileMock& operator=(const QZipFileMock& another) = delete;

    MOCK_METHOD(bool, open, (QIODevice::OpenMode mode), (override));
    MOCK_METHOD(QByteArray, readAll, (), (override));
    MOCK_METHOD(void, close, (), (override));
};
