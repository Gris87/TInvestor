#pragma once



#include <QWidget>



class IFilterWidget : public QWidget
{
public:
    explicit IFilterWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IFilterWidget() = default;

    IFilterWidget(const IFilterWidget& another)            = delete;
    IFilterWidget& operator=(const IFilterWidget& another) = delete;
};
