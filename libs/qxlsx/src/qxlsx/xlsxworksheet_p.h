// xlsxworksheet_p.h

#ifndef XLSXWORKSHEET_P_H
#define XLSXWORKSHEET_P_H

#include "src/qxlsx/xlsxabstractsheet_p.h"
#include "src/qxlsx/xlsxcell.h"
#include "src/qxlsx/xlsxcellformula.h"
#include "src/qxlsx/xlsxconditionalformatting.h"
#include "src/qxlsx/xlsxdatavalidation.h"
#include "src/qxlsx/xlsxworksheet.h"

#include <QHash>
#include <QImage>
#include <QObject>
#include <QRegularExpression>
#include <QString>
#include <QVector>

class QXmlStreamWriter;
class QXmlStreamReader;

QT_BEGIN_NAMESPACE_XLSX

class SharedStrings;

struct XlsxHyperlinkData
{
    enum LinkType
    {
        External,
        Internal
    };

    XlsxHyperlinkData(
        LinkType       linkType = External,
        const QString& target   = QString(),
        const QString& location = QString(),
        const QString& display  = QString(),
        const QString& tip      = QString()
    ) :
        linkType(linkType),
        target(target),
        location(location),
        display(display),
        tooltip(tip)
    {
    }

    LinkType linkType;
    QString  target; // For External link
    QString  location;
    QString  display;
    QString  tooltip;
};

// ECMA-376 Part1 18.3.1.81
struct XlsxSheetFormatProps
{
    XlsxSheetFormatProps(
        int    baseColWidth     = 8,
        bool   customHeight     = false,
        double defaultColWidth  = 8.430f, // https://learn.microsoft.com/en-us/office/troubleshoot/excel/determine-column-widths
        double defaultRowHeight = 15,
        quint8 outlineLevelCol  = 0,
        quint8 outlineLevelRow  = 0,
        bool   thickBottom      = false,
        bool   thickTop         = false,
        bool   zeroHeight       = false
    ) :
        baseColWidth(baseColWidth),
        customHeight(customHeight),
        defaultColWidth(defaultColWidth),
        defaultRowHeight(defaultRowHeight),
        outlineLevelCol(outlineLevelCol),
        outlineLevelRow(outlineLevelRow),
        thickBottom(thickBottom),
        thickTop(thickTop),
        zeroHeight(zeroHeight)
    {
    }

    int    baseColWidth;
    bool   customHeight;
    double defaultColWidth;
    double defaultRowHeight;
    quint8 outlineLevelCol;
    quint8 outlineLevelRow;
    bool   thickBottom;
    bool   thickTop;
    bool   zeroHeight;
};

struct XlsxRowInfo
{
    XlsxRowInfo(double height = 0, const Format& format = Format(), bool hidden = false) :
        customHeight(false),
        height(height),
        format(format),
        hidden(hidden),
        outlineLevel(0),
        collapsed(false)
    {
    }

    bool   customHeight;
    double height;
    Format format;
    bool   hidden;
    int    outlineLevel;
    bool   collapsed;
};

struct XlsxColumnInfo
{
    XlsxColumnInfo(
        int           firstColumn, // = 0,
        int           lastColumn,  // = 1,
        bool          isSetWidth,
        double        width  = 0,
        const Format& format = Format(),
        bool          hidden = false
    ) :
        width(width),
        format(format),
        firstColumn(firstColumn),
        lastColumn(lastColumn),
        outlineLevel(0),
        isSetWidth(isSetWidth),
        customWidth(false),
        hidden(hidden),
        collapsed(false)
    {
    }

    double width;
    Format format;
    int    firstColumn;
    int    lastColumn;
    int    outlineLevel;
    bool   isSetWidth;
    bool   customWidth;
    bool   hidden;
    bool   collapsed;
};

class CellTable
{
public:
    static QList<int> sorteIntList(QList<int>&& keys)
    {
        std::sort(keys.begin(), keys.end());
        return keys;
    }

    inline QList<int> sortedRows() const
    {
        QList<int> keys = cells.keys();
        std::sort(keys.begin(), keys.end());
        return keys;
    }

    void setValue(int row, int column, const std::shared_ptr<Cell>& cell)
    {
        cells[row].insert(column, cell);
        firstRow    = qMin(firstRow, row);
        firstColumn = qMin(firstColumn, column);
        lastRow     = qMin(lastRow, row);
        lastColumn  = qMin(lastColumn, column);
    }

    std::shared_ptr<Cell> cellAt(int row, int column) const
    {
        return cells.value(row).value(column);
    }

    bool contains(int row, int column) const
    {
        auto it = cells.find(row);
        if (it != cells.end())
        {
            return it->contains(column);
        }
        return false;
    }

    bool isEmpty() const
    {
        return cells.isEmpty();
    }

    // It's faster with a single QHash, but in Qt5 it's capacity limits
    // how much cells we can hold
    QHash<int, QHash<int, std::shared_ptr<Cell>>> cells;
    int                                           firstRow    = -1;
    int                                           firstColumn = -1;
    int                                           lastRow     = -1;
    int                                           lastColumn  = -1;
};

class WorksheetPrivate : public AbstractSheetPrivate
{
    Q_DECLARE_PUBLIC(Worksheet)

public:
    WorksheetPrivate(Worksheet* p, Worksheet::CreateFlag flag);
    ~WorksheetPrivate();

public:
    int     checkDimensions(int row, int col, bool ignore_row = false, bool ignore_col = false);
    Format  cellFormat(int row, int col) const;
    QString generateDimensionString() const;
    void    calculateSpans() const;
    void    splitColsInfo(int colFirst, int colLast);
    void    validateDimension();

    void saveXmlSheetData(QXmlStreamWriter& writer) const;
    void saveXmlCellData(QXmlStreamWriter& writer, int row, int col, std::shared_ptr<Cell> cell) const;
    void saveXmlMergeCells(QXmlStreamWriter& writer) const;
    void saveXmlHyperlinks(QXmlStreamWriter& writer) const;
    void saveXmlDrawings(QXmlStreamWriter& writer) const;
    void saveXmlDataValidations(QXmlStreamWriter& writer) const;

    int rowPixelsSize(int row) const;
    int colPixelsSize(int col) const;

    void loadXmlSheetData(QXmlStreamReader& reader);
    void loadXmlColumnsInfo(QXmlStreamReader& reader);
    void loadXmlMergeCells(QXmlStreamReader& reader);
    void loadXmlDataValidations(QXmlStreamReader& reader);
    void loadXmlSheetFormatProps(QXmlStreamReader& reader);
    void loadXmlSheetViews(QXmlStreamReader& reader);
    void loadXmlHyperlinks(QXmlStreamReader& reader);

    QList<std::shared_ptr<XlsxRowInfo>>    getRowInfoList(int rowFirst, int rowLast);
    QList<std::shared_ptr<XlsxColumnInfo>> getColumnInfoList(int colFirst, int colLast);
    QList<int>                             getColumnIndexes(int colFirst, int colLast);
    bool                                   isColumnRangeValid(int colFirst, int colLast);

    SharedStrings* sharedStrings() const;

public:
    CellTable cellTable;

    QHash<int, QHash<int, QString>>                            comments;
    QHash<int, QHash<int, std::shared_ptr<XlsxHyperlinkData>>> urlTable;
    QList<CellRange>                                           merges;
    QHash<int, std::shared_ptr<XlsxRowInfo>>                   rowsInfo;
    QHash<int, std::shared_ptr<XlsxColumnInfo>>                colsInfo;
    QHash<int, std::shared_ptr<XlsxColumnInfo>>                colsInfoHelper;

    QList<DataValidation>        dataValidationsList;
    QList<ConditionalFormatting> conditionalFormattingList;

    QHash<int, CellFormula> sharedFormulaMap; // shared formula map

    CellRange dimension;

    mutable QHash<int, QString> row_spans;
    QHash<int, double>          row_sizes;
    QHash<int, double>          col_sizes;

    // pagesetup and print settings add by liufeijin 20181028, liufeijin
    QString PpaperSize;
    QString Pscale;
    QString PfirstPageNumber;
    QString Porientation;
    QString PuseFirstPageNumber;
    QString PhorizontalDpi;
    QString PverticalDpi;
    QString Prid;
    QString Pcopies;

    // pageMargins, liufeijin
    QString PMheader;
    QString PMfooter;
    QString PMtop;
    QString PMbotton;
    QString PMleft;
    QString PMright;

    // header footer, liufeijin
    QString MoodFooter;
    QString ModdHeader;
    QString MoodalignWithMargins; // add align 20190619

    XlsxSheetFormatProps sheetFormatProps;

    bool windowProtection;
    bool showFormulas;
    bool showGridLines;
    bool showRowColHeaders;
    bool showZeros;
    bool rightToLeft;
    bool tabSelected;
    bool showRuler;
    bool showOutlineSymbols;
    bool showWhiteSpace;

    QRegularExpression urlPattern;

private:
    static double calculateColWidth(int characters);
};

QT_END_NAMESPACE_XLSX
#endif // XLSXWORKSHEET_P_H
