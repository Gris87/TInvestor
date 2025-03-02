#pragma once



#include "src/utils/fs/qzip/iqzip.h"



class QZip : public IQZip
{
public:
    QZip(const QString& name);
    ~QZip();

    QZip(const QZip& another)            = delete;
    QZip& operator=(const QZip& another) = delete;

    bool open(QuaZip::Mode mode) override;
    void close() override;

private:
    QuaZip mQuaZip;
};
