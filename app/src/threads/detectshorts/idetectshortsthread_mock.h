#pragma once



#include "src/threads/detectshorts/idetectshortsthread.h"

#include <gmock/gmock.h>



class DetectShortsThreadMock : public IDetectShortsThread
{
    Q_OBJECT

public:
    explicit DetectShortsThreadMock(QObject* parent = nullptr) :
        IDetectShortsThread(parent)
    {
    }
    ~DetectShortsThreadMock() override = default;

    DetectShortsThreadMock(const DetectShortsThreadMock& another)            = delete;
    DetectShortsThreadMock& operator=(const DetectShortsThreadMock& another) = delete;

    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, terminateThread, (), (override));
};
