// xlsxcelllocation.cpp

#include "src/qxlsx/xlsxcelllocation.h"

#include "src/qxlsx/xlsxcell.h"
#include "src/qxlsx/xlsxglobal.h"

#include <QList>
#include <QObject>
#include <QString>
#include <QVector>

QT_BEGIN_NAMESPACE_XLSX

CellLocation::CellLocation()
{
    col = -1;
    row = -1;

    cell.reset();
}

QT_END_NAMESPACE_XLSX
