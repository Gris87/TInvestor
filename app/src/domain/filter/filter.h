#pragma once



#include <QString>



struct Filter
{
    Filter();
    Filter(const Filter& another);
    ~Filter();

    Filter& operator=(const Filter& another);

    bool isFiltered(const QString& t, float price, float dayStartChange, float dateChange, float payback) const;

    bool    useTicker;
    QString ticker;
    bool    usePrice;
    float   priceFrom;
    float   priceTo;
    bool    useDayStartChange;
    float   dayStartChangeFrom;
    float   dayStartChangeTo;
    bool    useDateChange;
    float   dateChangeFrom;
    float   dateChangeTo;
    bool    usePayback;
    float   paybackFrom;
    float   paybackTo;
};



bool operator==(const Filter& lhs, const Filter& rhs);
