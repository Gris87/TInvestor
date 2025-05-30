#pragma once



#include <QWidget>

#include "src/domain/portfolio/portfolio.h"



class IPortfolioTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IPortfolioTreeWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IPortfolioTreeWidget() override = default;

    IPortfolioTreeWidget(const IPortfolioTreeWidget& another)            = delete;
    IPortfolioTreeWidget& operator=(const IPortfolioTreeWidget& another) = delete;

    virtual void portfolioChanged(const Portfolio& portfolio)               = 0;
    virtual void lastPriceChanged(const QString& instrumentId, float price) = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
