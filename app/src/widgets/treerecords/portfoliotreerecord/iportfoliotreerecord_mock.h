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
    MOCK_METHOD(void, updatePrice, (float price), (override));
    MOCK_METHOD(float, yield, (), (const, override));
    MOCK_METHOD(float, dailyYield, (), (const, override));
    MOCK_METHOD(void, exportToExcel, (QXlsx::Document & doc, int row), (const, override));
};
