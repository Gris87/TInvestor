#pragma once



#include <QWidget>

#include "src/domain/operation/operation.h"



class IOperationsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IOperationsTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IOperationsTableWidget() override = default;

    IOperationsTableWidget(const IOperationsTableWidget& another)            = delete;
    IOperationsTableWidget& operator=(const IOperationsTableWidget& another) = delete;

    virtual void operationsRead(const QList<Operation>& operations)  = 0;
    virtual void operationsAdded(const QList<Operation>& operations) = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
