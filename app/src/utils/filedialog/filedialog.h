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

    void setWindowTitle(const QString& title) override;
    void setAcceptMode(QFileDialog::AcceptMode mode) override;

private:
    QFileDialog mFileDialog;
};
