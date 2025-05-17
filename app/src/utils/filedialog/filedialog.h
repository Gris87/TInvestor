#pragma once



#include "src/utils/filedialog/ifiledialog.h"

#include <QFileDialog>



class FileDialog : public IFileDialog
{
public:
    explicit FileDialog(QWidget* parent);
    ~FileDialog() override;

    FileDialog(const FileDialog& another)            = delete;
    FileDialog& operator=(const FileDialog& another) = delete;

private:
    QFileDialog mFileDialog;
};
