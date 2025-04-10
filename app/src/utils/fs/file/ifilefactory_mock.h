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

    MOCK_METHOD(std::shared_ptr<IFile>, newInstance, (const QString& name), (const, override));
};
