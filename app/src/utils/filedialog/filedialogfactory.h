#pragma once



#include "src/utils/filedialog/ifiledialogfactory.h"



class FileDialogFactory : public IFileDialogFactory
{
public:
    FileDialogFactory();
    ~FileDialogFactory() override;

    FileDialogFactory(const FileDialogFactory& another)            = delete;
    FileDialogFactory& operator=(const FileDialogFactory& another) = delete;

    std::shared_ptr<IFileDialog>
    newInstance(QWidget* parent, const QString& caption, const QString& directory, const QString& filter) const override;
};
