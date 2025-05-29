#include "src/widgets/tableitems/instrument/instrumenttableitemwidget.h"

#include <QDebug>



InstrumentTableItemWidget::InstrumentTableItemWidget(IUserStorage* userStorage, QWidget* parent) :
    IInstrumentTableItemWidget(userStorage, parent)
{
    qDebug() << "Create InstrumentTableItemWidget";
}

InstrumentTableItemWidget::~InstrumentTableItemWidget()
{
    qDebug() << "Destroy InstrumentTableItemWidget";
}

bool InstrumentTableItemWidget::operator<(const QTableWidgetItem& another) const
{
    const QString anotherName = dynamic_cast<const InstrumentTableItemWidget*>(&another)->ticker();

    return ticker() < anotherName;
}
