#pragma once



#include "src/utils/fs/qzip/iqzipfactory.h"



class QZipFactory : public IQZipFactory
{
public:
    QZipFactory();
    ~QZipFactory();

    QZipFactory(const QZipFactory& another)            = delete;
    QZipFactory& operator=(const QZipFactory& another) = delete;

    std::shared_ptr<IQZip> newInstance(const QString& name) override;
};
