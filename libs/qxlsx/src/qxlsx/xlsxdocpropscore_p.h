// xlsxdocpropscore_p.h

#ifndef XLSXDOCPROPSCORE_H
#define XLSXDOCPROPSCORE_H

#include "src/qxlsx/xlsxabstractooxmlfile.h"
#include "src/qxlsx/xlsxglobal.h"

#include <QMap>
#include <QStringList>

class QIODevice;

QT_BEGIN_NAMESPACE_XLSX

class DocPropsCore : public AbstractOOXmlFile
{
public:
    explicit DocPropsCore(CreateFlag flag);

    bool        setProperty(const QString& name, const QString& value);
    QString     property(const QString& name) const;
    QStringList propertyNames() const;

    void saveToXmlFile(QIODevice* device) const override;
    bool loadFromXmlFile(QIODevice* device) override;

private:
    QMap<QString, QString> m_properties;
};

QT_END_NAMESPACE_XLSX

#endif // XLSXDOCPROPSCORE_H
