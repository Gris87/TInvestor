// xlsxdrwaing_p.h

#ifndef QXLSX_DRAWING_H
#define QXLSX_DRAWING_H

#include "src/qxlsx/xlsxabstractooxmlfile.h"
#include "src/qxlsx/xlsxrelationships_p.h"

#include <QList>
#include <QString>

class QIODevice;
class QXmlStreamWriter;

QT_BEGIN_NAMESPACE_XLSX

class DrawingAnchor;
class Workbook;
class AbstractSheet;
class MediaFile;

class Drawing : public AbstractOOXmlFile
{
public:
    Drawing(AbstractSheet* sheet, CreateFlag flag);
    ~Drawing();
    void saveToXmlFile(QIODevice* device) const override;
    bool loadFromXmlFile(QIODevice* device) override;

    AbstractSheet*        sheet;
    Workbook*             workbook;
    QList<DrawingAnchor*> anchors;
};

QT_END_NAMESPACE_XLSX

#endif // QXLSX_DRAWING_H
