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
};
