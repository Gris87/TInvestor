// xlsxdocument_p.h

#ifndef XLSXDOCUMENT_P_H
#define XLSXDOCUMENT_P_H

#include "src/qxlsx/xlsxcontenttypes_p.h"
#include "src/qxlsx/xlsxdocument.h"
#include "src/qxlsx/xlsxglobal.h"
#include "src/qxlsx/xlsxworkbook.h"

#include <QMap>

QT_BEGIN_NAMESPACE_XLSX

class DocumentPrivate
{
    Q_DECLARE_PUBLIC(Document)

public:
    DocumentPrivate(Document* p);
    void init();

    bool loadPackage(QIODevice* device);
    bool savePackage(QIODevice* device) const;

    bool saveCsv(const QString mainCSVFileName) const;

    // copy style from one xlsx file to other
    static bool copyStyle(const QString& from, const QString& to);

    Document*     q_ptr;
    const QString defaultPackageName; // default name when package name not specified
    QString       packageName;        // name of the .xlsx file

    QMap<QString, QString>        documentProperties; // core, app and custom properties
    std::shared_ptr<Workbook>     workbook;
    std::shared_ptr<ContentTypes> contentTypes;
    bool                          isLoad;
};

QT_END_NAMESPACE_XLSX

#endif // XLSXDOCUMENT_P_H
