#pragma once



#include "src/utils/filedialog/ifiledialogfactory.h"

#include <gmock/gmock.h>



class FileDialogFactoryMock : public IFileDialogFactory
{
public:
    FileDialogFactoryMock() :
        IFileDialogFactory()
    {
    }
    ~FileDialogFactoryMock() override = default;

    FileDialogFactoryMock(const FileDialogFactoryMock& another)            = delete;
    FileDialogFactoryMock& operator=(const FileDialogFactoryMock& another) = delete;

    MOCK_METHOD(
        std::shared_ptr<IFileDialog>,
        newInstance,
        (QWidget * parent, const QString& caption, const QString& directory, const QString& filter),
        (const, override)
    );
};
