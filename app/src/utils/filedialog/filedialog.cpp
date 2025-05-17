#include "src/utils/filedialog/filedialog.h"

#include <QDebug>



FileDialog::FileDialog() :
    IFileDialog()
{
    qDebug() << "Create FileDialog";
}

FileDialog::~FileDialog()
{
    qDebug() << "Destroy FileDialog";
}
