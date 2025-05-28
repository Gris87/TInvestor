#pragma once



#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"

#include <gmock/gmock.h>



class PortfolioTreeRecordMock : public IPortfolioTreeRecord
{
    Q_OBJECT

public:
    explicit PortfolioTreeRecordMock(QObject* parent = nullptr) :
        IPortfolioTreeRecord(parent)
    {
    }
    ~PortfolioTreeRecordMock() override = default;

    PortfolioTreeRecordMock(const PortfolioTreeRecordMock& another)            = delete;
    PortfolioTreeRecordMock& operator=(const PortfolioTreeRecordMock& another) = delete;

    MOCK_METHOD(void, setPortfolioItem, (const PortfolioItem& item), (override));
};
