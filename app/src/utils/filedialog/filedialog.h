#pragma once



#include "src/utils/filedialog/ifiledialog.h"

#include <QFileDialog>



class FileDialog : public IFileDialog
{
public:
    explicit FileDialog(QWidget* parent, const QString& caption, const QString& directory, const QString& filter);
    ~FileDialog() override;

    FileDialog(const FileDialog& another)            = delete;
    FileDialog& operator=(const FileDialog& another) = delete;

    void        setAcceptMode(QFileDialog::AcceptMode mode) override;
    void        selectFile(const QString& filename) override;
    int         exec() override;
    QStringList selectedFiles() const override;

private:
    QFileDialog mFileDialog;
};
