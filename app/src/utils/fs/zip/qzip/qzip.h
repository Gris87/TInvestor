#pragma once



#include "src/utils/fs/zip/qzip/iqzip.h"



class QZip : public IQZip
{
public:
    explicit QZip(const QString& name);
    ~QZip();

    QZip(const QZip& another)            = delete;
    QZip& operator=(const QZip& another) = delete;

    QuaZip* getZip() override;

    bool open(QuaZip::Mode mode) override;
    void close() override;

    QStringList getFileNameList() override;
    bool        setCurrentFile(const QString& fileName) override;

private:
    QuaZip mQuaZip;
};
