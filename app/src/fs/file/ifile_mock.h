#pragma once



#include "src/fs/file/ifile.h"

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

    MOCK_METHOD1(open, bool(QIODevice::OpenMode flags));
    MOCK_METHOD2(read, qint64(char* data, qint64 maxlen));
    MOCK_METHOD0(readAll, QByteArray());
    MOCK_METHOD2(write, qint64(const char* data, qint64 len));
    MOCK_METHOD1(write, qint64(const QByteArray& data));
    MOCK_METHOD0(size, qint64());
    MOCK_METHOD0(close, void());
};
