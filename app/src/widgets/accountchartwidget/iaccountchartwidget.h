#pragma once



#include <QGraphicsView>



class IAccountChartWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit IAccountChartWidget(QWidget* parent = nullptr) :
        QGraphicsView(parent)
    {
    }
    virtual ~IAccountChartWidget() = default;

    IAccountChartWidget(const IAccountChartWidget& another)            = delete;
    IAccountChartWidget& operator=(const IAccountChartWidget& another) = delete;
};
