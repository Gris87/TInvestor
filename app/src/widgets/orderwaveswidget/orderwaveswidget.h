#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidget.h"



class OrderWavesWidget : public IOrderWavesWidget
{
public:
    explicit OrderWavesWidget(QWidget* parent = nullptr);
    ~OrderWavesWidget();

    OrderWavesWidget(const OrderWavesWidget& another)            = delete;
    OrderWavesWidget& operator=(const OrderWavesWidget& another) = delete;
};
