#include "src/utils/filedialog/filedialog.h"

#include <QDebug>



FileDialog::FileDialog(QWidget* parent, const QString& caption, const QString& directory, const QString& filter) :
    IFileDialog(),
    mFileDialog(parent, caption, directory, filter)
{
    qDebug() << "Create FileDialog";
}

FileDialog::~FileDialog()
{
    qDebug() << "Destroy FileDialog";
}

void FileDialog::setAcceptMode(QFileDialog::AcceptMode mode)
{
    mFileDialog.setAcceptMode(mode);
}

void FileDialog::selectFile(const QString& filename)
{
    mFileDialog.selectFile(filename);
}

int FileDialog::exec()
{
    return mFileDialog.exec();
}

QStringList FileDialog::selectedFiles() const
{
    return mFileDialog.selectedFiles();
}
