#include "src/utils/optimizer/optimizer.h"

#include <QDebug>



Optimizer::Optimizer() :
    IOptimizer()
{
    qDebug() << "Create Optimizer";
}

Optimizer::~Optimizer()
{
    qDebug() << "Destroy Optimizer";
}

void Optimizer::optimizeOperations()
{
}

void Optimizer::optimizeLogs()
{
}
