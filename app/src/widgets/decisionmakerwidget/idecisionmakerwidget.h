#pragma once



#include <QWidget>

#include "src/domain/operation/operation.h"



class IDecisionMakerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IDecisionMakerWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IDecisionMakerWidget() override = default;

    IDecisionMakerWidget(const IDecisionMakerWidget& another)            = delete;
    IDecisionMakerWidget& operator=(const IDecisionMakerWidget& another) = delete;

    virtual void operationsRead(const QList<Operation>& operations)  = 0;
    virtual void operationsAdded(const QList<Operation>& operations) = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
