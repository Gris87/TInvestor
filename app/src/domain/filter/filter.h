#pragma once



#include <QString>



struct Filter
{
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
