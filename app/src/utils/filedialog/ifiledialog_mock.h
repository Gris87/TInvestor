#pragma once



#include "src/utils/filedialog/ifiledialog.h"

#include <gmock/gmock.h>



class FileDialogMock : public IFileDialog
{
public:
    FileDialogMock() :
        IFileDialog()
    {
    }
    ~FileDialogMock() override = default;

    FileDialogMock(const FileDialogMock& another)            = delete;
    FileDialogMock& operator=(const FileDialogMock& another) = delete;

    MOCK_METHOD(void, setAcceptMode, (QFileDialog::AcceptMode mode), (override));
    MOCK_METHOD(void, setDefaultSuffix, (const QString& suffix), (override));
    MOCK_METHOD(void, selectFile, (const QString& filename), (override));
    MOCK_METHOD(int, exec, (), (override));
    MOCK_METHOD(QStringList, selectedFiles, (), (const, override));
};
