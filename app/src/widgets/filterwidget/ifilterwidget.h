#pragma once



#include <QWidget>

#include "src/domain/filter/filter.h"



class IFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IFilterWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IFilterWidget() = default;

    IFilterWidget(const IFilterWidget& another)            = delete;
    IFilterWidget& operator=(const IFilterWidget& another) = delete;

    virtual const Filter& getFilter() = 0;

signals:
    void dateChangeDateTimeChanged();
    void filterChanged(const Filter& filter);
};
