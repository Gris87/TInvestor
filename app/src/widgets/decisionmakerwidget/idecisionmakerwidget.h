#pragma once



#include <QWidget>

#include "src/domain/operation/operation.h"
#include "src/domain/portfolio/portfolio.h"



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

    virtual void setAccountName(const QString& name) = 0;

    virtual void operationsRead(const QList<Operation>& operations)         = 0;
    virtual void operationsAdded(const QList<Operation>& operations)        = 0;
    virtual void portfolioChanged(const Portfolio& portfolio)               = 0;
    virtual void lastPriceChanged(const QString& instrumentId, float price) = 0;
    virtual void updateLastPrices()                                         = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
