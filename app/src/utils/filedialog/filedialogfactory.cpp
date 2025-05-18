#include "src/utils/filedialog/filedialogfactory.h"

#include <QDebug>

#include "src/utils/filedialog/filedialog.h"



FileDialogFactory::FileDialogFactory() :
    IFileDialogFactory()
{
    qDebug() << "Create FileDialogFactory";
}

FileDialogFactory::~FileDialogFactory()
{
    qDebug() << "Destroy FileDialogFactory";
}

std::shared_ptr<IFileDialog>
FileDialogFactory::newInstance(QWidget* parent, const QString& caption, const QString& directory, const QString& filter) const
{
    return std::shared_ptr<IFileDialog>(new FileDialog(parent, caption, directory, filter));
}
