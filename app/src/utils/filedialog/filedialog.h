#pragma once



#include "src/utils/filedialog/ifiledialog.h"



class FileDialog : public IFileDialog
{
public:
    explicit FileDialog();
    ~FileDialog() override;

    FileDialog(const FileDialog& another)            = delete;
    FileDialog& operator=(const FileDialog& another) = delete;
};
