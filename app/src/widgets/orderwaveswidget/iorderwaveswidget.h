#pragma once



#include <QGraphicsView>



class IOrderWavesWidget : public QGraphicsView
{
public:
    explicit IOrderWavesWidget(QWidget* parent = nullptr) :
        QGraphicsView(parent)
    {
    }
    virtual ~IOrderWavesWidget() = default;

    IOrderWavesWidget(const IOrderWavesWidget& another)            = delete;
    IOrderWavesWidget& operator=(const IOrderWavesWidget& another) = delete;
};
