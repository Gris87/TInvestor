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

float Bollindger::getTopEdge() const
{
    return 0;
}

float Bollindger::getBottomEdge() const
{
    return 0;
}
