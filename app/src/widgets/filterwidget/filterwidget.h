#pragma once



#include "src/widgets/filterwidget/ifilterwidget.h"



namespace Ui
{
class FilterWidget;
}



class FilterWidget : public IFilterWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget* parent = nullptr);
    ~FilterWidget();

    FilterWidget(const FilterWidget& another)            = delete;
    FilterWidget& operator=(const FilterWidget& another) = delete;

    Ui::FilterWidget* ui;
};
