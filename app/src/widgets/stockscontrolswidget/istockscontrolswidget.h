#pragma once



#include <QWidget>

#include <QDateTime>

#include "src/domain/filter/filter.h"



class IStocksControlsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IStocksControlsWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IStocksControlsWidget() = default;

    IStocksControlsWidget(const IStocksControlsWidget& another)            = delete;
    IStocksControlsWidget& operator=(const IStocksControlsWidget& another) = delete;

    virtual QDateTime     getDateChangeTime() = 0;
    virtual const Filter& getFilter()         = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;

signals:
    void dateChangeDateTimeChanged(const QDateTime& dateTime);
    void filterChanged(const Filter& filter);
};
