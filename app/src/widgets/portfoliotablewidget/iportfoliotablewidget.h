#pragma once



#include <QWidget>

#include "src/domain/portfolio/portfolio.h"



class IPortfolioTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IPortfolioTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IPortfolioTableWidget() override = default;

    IPortfolioTableWidget(const IPortfolioTableWidget& another)            = delete;
    IPortfolioTableWidget& operator=(const IPortfolioTableWidget& another) = delete;

    virtual void portfolioChanged(const Portfolio& portfolio) = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
