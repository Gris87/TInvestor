#include "src/utils/bollindger/bollindger.h"

#include <QDebug>



Bollindger::Bollindger() :
    IBollindger()
{
    qDebug() << "Create Bollindger";
}

Bollindger::~Bollindger()
{
    qDebug() << "Destroy Bollindger";
}

float Bollindger::getTopEdge(Stock* stock, int startIndex, int endIndex) const
{
    if (startIndex >= endIndex)
    {
        return -1;
    }

    const double movingAverage     = getMovingAverage(stock, startIndex, endIndex);
    const double standartDeviation = getStandartDeviation(stock, startIndex, endIndex, movingAverage);

    return movingAverage + (2 * standartDeviation);
}

float Bollindger::getBottomEdge(Stock* stock, int startIndex, int endIndex) const
{
    if (startIndex >= endIndex)
    {
        return -1;
    }

    const double movingAverage     = getMovingAverage(stock, startIndex, endIndex);
    const double standartDeviation = getStandartDeviation(stock, startIndex, endIndex, movingAverage);

    return movingAverage - (2 * standartDeviation);
}

double Bollindger::getMovingAverage(Stock* stock, int startIndex, int endIndex) const
{
    double total = 0.0;

    for (int i = startIndex; i < endIndex; ++i)
    {
        total += stock->data.at(i).price;
    }

    return total / (endIndex - startIndex);
}

double Bollindger::getStandartDeviation(Stock* stock, int startIndex, int endIndex, double mean) const
{
    double total = 0.0;

    for (int i = startIndex; i < endIndex; ++i)
    {
        const double price = stock->data.at(i).price;

        total += (price - mean) * (price - mean);
    }

    return qSqrt(total / (endIndex - startIndex + 1));
}

float Bollindger::getTopEdgeOperational(Stock* stock, int startIndex, int endIndex) const
{
    if (startIndex >= endIndex)
    {
        return -1;
    }

    const double movingAverage     = getMovingAverageOperational(stock, startIndex, endIndex);
    const double standartDeviation = getStandartDeviationOperational(stock, startIndex, endIndex, movingAverage);

    return movingAverage + (2 * standartDeviation);
}

float Bollindger::getBottomEdgeOperational(Stock* stock, int startIndex, int endIndex) const
{
    if (startIndex >= endIndex)
    {
        return -1;
    }

    const double movingAverage     = getMovingAverageOperational(stock, startIndex, endIndex);
    const double standartDeviation = getStandartDeviationOperational(stock, startIndex, endIndex, movingAverage);

    return movingAverage - (2 * standartDeviation);
}

double Bollindger::getMovingAverageOperational(Stock* stock, int startIndex, int endIndex) const
{
    double total = 0.0;

    for (int i = startIndex; i < endIndex; ++i)
    {
        total += stock->operational.detailedData.at(i).price;
    }

    return total / (endIndex - startIndex);
}

double Bollindger::getStandartDeviationOperational(Stock* stock, int startIndex, int endIndex, double mean) const
{
    double total = 0.0;

    for (int i = startIndex; i < endIndex; ++i)
    {
        const double price = stock->operational.detailedData.at(i).price;

        total += (price - mean) * (price - mean);
    }

    return qSqrt(total / (endIndex - startIndex + 1));
}
