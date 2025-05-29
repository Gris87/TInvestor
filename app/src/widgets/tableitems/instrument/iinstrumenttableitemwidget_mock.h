#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget.h"

#include <gmock/gmock.h>



class InstrumentTableItemWidgetMock : public IInstrumentTableItemWidget
{
    Q_OBJECT

public:
    explicit InstrumentTableItemWidgetMock(IUserStorage* userStorage, QWidget* parent = nullptr) :
        IInstrumentTableItemWidget(userStorage, parent)
    {
    }
    ~InstrumentTableItemWidgetMock() override = default;

    InstrumentTableItemWidgetMock(const InstrumentTableItemWidgetMock& another)            = delete;
    InstrumentTableItemWidgetMock& operator=(const InstrumentTableItemWidgetMock& another) = delete;

    MOCK_METHOD(void, setInstrumentLogo, (const QIcon& icon), (override));
    MOCK_METHOD(void, setQualInvestor, (bool forQualInvestorFlag), (override));
    MOCK_METHOD(void, setTicker, (const QString& text), (override));
    MOCK_METHOD(void, setFullText, (const QString& text), (override));

    MOCK_METHOD(bool, forQualInvestorFlag, (), (const, override));
    MOCK_METHOD(QString, ticker, (), (const, override));
    MOCK_METHOD(QString, fullText, (), (const, override));
};
