// xlsxcellformula_p.h

#ifndef XLSXCELLFORMULA_P_H
#define XLSXCELLFORMULA_P_H

#include "src/qxlsx/xlsxcellformula.h"
#include "src/qxlsx/xlsxcellrange.h"
#include "src/qxlsx/xlsxglobal.h"

#include <QSharedData>
#include <QString>

QT_BEGIN_NAMESPACE_XLSX

class CellFormulaPrivate : public QSharedData
{
public:
    CellFormulaPrivate(const QString& formula, const CellRange& reference, CellFormula::FormulaType type);
    CellFormulaPrivate(const CellFormulaPrivate& other);
    ~CellFormulaPrivate();

    QString                  formula; // formula contents
    CellFormula::FormulaType type;
    CellRange                reference;
    bool                     ca; // Calculate Cell
    int                      si; // Shared group index
};

QT_END_NAMESPACE_XLSX

#endif // XLSXCELLFORMULA_P_H
