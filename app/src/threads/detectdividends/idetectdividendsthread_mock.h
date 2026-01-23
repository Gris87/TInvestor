#pragma once



#include "src/threads/detectdividends/idetectdividendsthread.h"

#include <gmock/gmock.h>



class DetectDividendsThreadMock : public IDetectDividendsThread
{
    Q_OBJECT

public:
    explicit DetectDividendsThreadMock(QObject* parent = nullptr) :
        IDetectDividendsThread(parent)
    {
    }
    ~DetectDividendsThreadMock() override = default;

    DetectDividendsThreadMock(const DetectDividendsThreadMock& another)            = delete;
    DetectDividendsThreadMock& operator=(const DetectDividendsThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
