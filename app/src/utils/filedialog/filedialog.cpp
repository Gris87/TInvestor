#include "src/utils/filedialog/filedialog.h"

#include <QDebug>



FileDialog::FileDialog(QWidget* parent) :
    IFileDialog(),
    mFileDialog(parent)
{
    qDebug() << "Create FileDialog";
}

FileDialog::~FileDialog()
{
    qDebug() << "Destroy FileDialog";
}
