#pragma once



#include "src/fs/file/ifilefactory.h"

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

    MOCK_METHOD0(newInstance, IFile*());
};
