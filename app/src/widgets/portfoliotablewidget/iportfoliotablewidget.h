#pragma once



#include <QWidget>



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

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
