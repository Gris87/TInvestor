#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget.h"

#include <gmock/gmock.h>



class InstrumentTableItemWidgetMock : public IInstrumentTableItemWidget
{
    Q_OBJECT

public:
    explicit InstrumentTableItemWidgetMock(QWidget* parent = nullptr) :
        IInstrumentTableItemWidget(parent)
    {
    }
    ~InstrumentTableItemWidgetMock() override = default;

    InstrumentTableItemWidgetMock(const InstrumentTableItemWidgetMock& another)            = delete;
    InstrumentTableItemWidgetMock& operator=(const InstrumentTableItemWidgetMock& another) = delete;

    MOCK_METHOD(void, setIcon, (const QIcon& icon), (override));
    MOCK_METHOD(void, setQualInvestor, (bool forQualInvestorFlag), (override));
    MOCK_METHOD(void, setText, (const QString& text), (override));
    MOCK_METHOD(void, setFullText, (const QString& text), (override));

    MOCK_METHOD(bool, forQualInvestorFlag, (), (override));
    MOCK_METHOD(QString, text, (), (override));
    MOCK_METHOD(QString, fullText, (), (override));
};
