#pragma once



#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget.h"



class InstrumentTableItemWidget : public IInstrumentTableItemWidget
{
    Q_OBJECT

public:
    explicit InstrumentTableItemWidget(IUserStorage* userStorage, QWidget* parent = nullptr);
    ~InstrumentTableItemWidget() override;

    InstrumentTableItemWidget(const InstrumentTableItemWidget& another)            = delete;
    InstrumentTableItemWidget& operator=(const InstrumentTableItemWidget& another) = delete;

    bool operator<(const QTableWidgetItem& another) const override;
};
