#pragma once



#include "src/widgets/instrumentwidget/iinstrumentwidget.h"

#include <gmock/gmock.h>



class InstrumentWidgetMock : public IInstrumentWidget
{
    Q_OBJECT

public:
    explicit InstrumentWidgetMock(QWidget* parent = nullptr) :
        IInstrumentWidget(parent)
    {
    }
    ~InstrumentWidgetMock() override = default;

    InstrumentWidgetMock(const InstrumentWidgetMock& another)            = delete;
    InstrumentWidgetMock& operator=(const InstrumentWidgetMock& another) = delete;

    MOCK_METHOD(void, setIcon, (const QIcon& icon), (override));
    MOCK_METHOD(void, setQualInvestor, (bool forQualInvestorFlag), (override));
    MOCK_METHOD(void, setText, (const QString& text), (override));
    MOCK_METHOD(void, setFullText, (const QString& text), (override));

    MOCK_METHOD(bool, forQualInvestorFlag, (), (override));
    MOCK_METHOD(QString, text, (), (override));
    MOCK_METHOD(QString, fullText, (), (override));
};
