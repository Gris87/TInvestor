#pragma once



#include "src/utils/fs/zip/qzip/iqzipfactory.h"



class QZipFactory : public IQZipFactory
{
public:
    QZipFactory();
    ~QZipFactory() override;

    QZipFactory(const QZipFactory& another)            = delete;
    QZipFactory& operator=(const QZipFactory& another) = delete;

    [[nodiscard]]
    std::shared_ptr<IQZip> newInstance(QIODevice* device) const override;
};
