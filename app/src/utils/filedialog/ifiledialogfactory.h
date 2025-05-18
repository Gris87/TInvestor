#pragma once



#include <QWidget>

#include "src/utils/filedialog/ifiledialog.h"



class IFileDialogFactory
{
public:
    IFileDialogFactory()          = default;
    virtual ~IFileDialogFactory() = default;

    IFileDialogFactory(const IFileDialogFactory& another)            = delete;
    IFileDialogFactory& operator=(const IFileDialogFactory& another) = delete;

    virtual std::shared_ptr<IFileDialog>
    newInstance(QWidget* parent, const QString& caption, const QString& directory, const QString& filter) const = 0;
};
