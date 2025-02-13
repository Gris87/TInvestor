#pragma once



#include "src/utils/fs/file/ifile.h"

#include <gmock/gmock.h>



class FileMock : public IFile
{
public:
    FileMock() :
        IFile()
    {
    }
    ~FileMock() override = default;

    FileMock(const FileMock& another)            = delete;
    FileMock& operator=(const FileMock& another) = delete;

    MOCK_METHOD(bool, open, (QIODevice::OpenMode flags), (override));
    MOCK_METHOD(qint64, read, (char* data, qint64 maxlen), (override));
    MOCK_METHOD(QByteArray, readAll, (), (override));
    MOCK_METHOD(qint64, write, (const char* data, qint64 len), (override));
    MOCK_METHOD(qint64, write, (const QByteArray& data), (override));
    MOCK_METHOD(qint64, size, (), (override));
    MOCK_METHOD(void, close, (), (override));
};
