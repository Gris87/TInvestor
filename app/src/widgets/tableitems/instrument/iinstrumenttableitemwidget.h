#pragma once



#include <QTableWidgetItem>

#include "src/widgets/instrumentwidget/instrumentwidget.h"



class IInstrumentTableItemWidget : public InstrumentWidget,
                                   public QTableWidgetItem
{
    Q_OBJECT

public:
    explicit IInstrumentTableItemWidget(IUserStorage* userStorage, QWidget* parent = nullptr) :
        InstrumentWidget(userStorage, parent),
        QTableWidgetItem()
    {
    }
    ~IInstrumentTableItemWidget() override = default;

    IInstrumentTableItemWidget(const IInstrumentTableItemWidget& another)            = delete;
    IInstrumentTableItemWidget& operator=(const IInstrumentTableItemWidget& another) = delete;
};
