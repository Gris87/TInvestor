#pragma once



#include <QGraphicsView>

#include "src/domain/operation/operation.h"



class IAccountChartWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit IAccountChartWidget(QWidget* parent = nullptr) :
        QGraphicsView(parent)
    {
    }
    ~IAccountChartWidget() override = default;

    IAccountChartWidget(const IAccountChartWidget& another)            = delete;
    IAccountChartWidget& operator=(const IAccountChartWidget& another) = delete;

    virtual void operationsRead(const QList<Operation>& operations)  = 0;
    virtual void operationsAdded(const QList<Operation>& operations) = 0;
};
