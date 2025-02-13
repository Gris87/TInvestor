#pragma once



#include "src/utils/fs/file/ifilefactory.h"

#include <gmock/gmock.h>



class FileFactoryMock : public IFileFactory
{
public:
    FileFactoryMock() :
        IFileFactory()
    {
    }
    ~FileFactoryMock() override = default;

    FileFactoryMock(const FileFactoryMock& another)            = delete;
    FileFactoryMock& operator=(const FileFactoryMock& another) = delete;

    MOCK_METHOD(IFile*, newInstance, (const QString& name), (override));
};
