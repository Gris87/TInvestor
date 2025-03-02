#pragma once



#include "src/utils/fs/qzip/iqzip.h"

#include <gmock/gmock.h>



class QZipMock : public IQZip
{
public:
    QZipMock() :
        IQZip()
    {
    }
    ~QZipMock() override = default;

    QZipMock(const QZipMock& another)            = delete;
    QZipMock& operator=(const QZipMock& another) = delete;
};
