#pragma once



#include <QWidget>

#include <QDateTime>

#include "src/domain/filter/logfilter.h"



class ILogsFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ILogsFilterWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ILogsFilterWidget() override = default;

    ILogsFilterWidget(const ILogsFilterWidget& another)            = delete;
    ILogsFilterWidget& operator=(const ILogsFilterWidget& another) = delete;

    virtual const LogFilter& getFilter() = 0;

signals:
    void filterChanged(const LogFilter& filter);
};
