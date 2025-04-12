#pragma once



#include "src/utils/fs/zip/qzipfile/iqzipfilefactory.h"



class QZipFileFactory : public IQZipFileFactory
{
public:
    QZipFileFactory();
    ~QZipFileFactory() override;

    QZipFileFactory(const QZipFileFactory& another)            = delete;
    QZipFileFactory& operator=(const QZipFileFactory& another) = delete;

    std::shared_ptr<IQZipFile> newInstance(QuaZip* zip) const override;
};
