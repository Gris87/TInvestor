#pragma once



#include <QString>



enum QualInvestorFilter : quint8
{
    QUAL_INVESTOR_SHOW_ALL,
    QUAL_INVESTOR_ONLY_WITH_STATUS,
    QUAL_INVESTOR_ONLY_WITHOUT_STATUS
};



struct Filter
{
    Filter();
    Filter(const Filter& another) = default;
    ~Filter()                     = default;

    Filter& operator=(const Filter& another) = default;

    [[nodiscard]]
    bool isActive() const;

    [[nodiscard]]
    bool isFiltered(
        const QString& t,
        const QString& name,
        bool           forQualInvestorFlag,
        float          price,
        float          dayStartChange,
        float          dateChange,
        qint64         turnover,
        float          payback
    ) const;

    bool               useTicker;
    QString            ticker;
    bool               useQualInvestor;
    QualInvestorFilter qualInvestor;
    bool               usePrice;
    float              priceFrom;
    float              priceTo;
    bool               useDayStartChange;
    float              dayStartChangeFrom;
    float              dayStartChangeTo;
    bool               useDateChange;
    float              dateChangeFrom;
    float              dateChangeTo;
    bool               useTurnover;
    qint64             turnoverFrom;
    qint64             turnoverTo;
    bool               usePayback;
    float              paybackFrom;
    float              paybackTo;
};



bool operator==(const Filter& lhs, const Filter& rhs);
