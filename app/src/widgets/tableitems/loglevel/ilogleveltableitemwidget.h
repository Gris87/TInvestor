#pragma once



#include <QTableWidgetItem>
#include <QWidget>

#include "src/domain/log/logentry.h"



class ILogLevelTableItemWidget : public QWidget,
                                 public QTableWidgetItem
{
    Q_OBJECT

public:
    explicit ILogLevelTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ILogLevelTableItemWidget() override = default;

    ILogLevelTableItemWidget(const ILogLevelTableItemWidget& another)            = delete;
    ILogLevelTableItemWidget& operator=(const ILogLevelTableItemWidget& another) = delete;

    virtual void     setLogLevel(LogLevel level) = 0;
    virtual LogLevel logLevel() const            = 0;
};
